// HippoBrain.cpp : Defines the entry point for the application.
#include "HippoBrain.h"

using Brain = HippoBrain;

HippoBrain::HippoBrain(std::initializer_list<char> types,
	std::initializer_list<float> thresholds,
	std::initializer_list<LinkInitializer> linksTime,
	std::initializer_list<LinkInitializer> linksUp):
	SemanticNN(types, thresholds, linksTime, linksUp)
{
}
void HippoBrain::finalizeCreation() {
	for (PNC nc : {_askLoadSensors, _return, _memorizeCurrentState,
		_hippoLeftEnd, _hippoRightEnd, _shiftLeft, _shiftRight, _strictTuringMode})
	{
		nc->_isSystem = true;
	}
	_askLoadSensors->nIn.activate();
	_askLoadSensors->nPrev.activate();
}
void Brain::inferenceCycle() {
	while (!_needExit)
		inferenceStep();
}
void Brain::cycleUntillEnough() { // used in Unit Testing
	do {
		inferenceStep();
	} while (_activeChanged && !_needExit);
}

void Brain::inferenceStep() {
	if (_return->nIn.isActive()) {
		callReturn();
	} else {
		for(NC* nc: _clusters) { // swap cur/next cycle activation
			nc->nPrev = nc->nIn.isActive() ? 1.f : 0.f;
			nc->_sent = false;
			if (nc->_type == NC::TypePermanent) {
				if (!nc->nIn.isActive())
					nc->nIn = 0;
			} else {
				nc->nIn = 0;
			}
			nc->nIn.clipToThreshold();
		}
	}

	++_cycle;
	++_cycleForCurrentToken;

	// time link directions
	for(NC* nc: _clusters) {
		if (nc->nPrev.isActive()) {
			for (Link* link: nc->lTOut) {
				link->_to.nIn += link->b;
			}
		}
	}

	if (_shiftLeft->nPrev.isActive()) {
		shiftLeft();
	} else if (_shiftRight->nPrev.isActive()) {
		shiftRight();
	} else if (_activateMemory->nPrev.isActive()) {
		// activate
		for (Link* link : _hippo->lUpIn) {
			link->_from.nIn.activate();
		}
	} else if (_askLoadSensors->nIn.isActive() || !_activeChanged) {
		loadSensors(); // time for new input
	}

	for (int virtualLayer = 0; !isExceptionActive(); ++virtualLayer) {
		if (virtualLayer > 100) {
			assert(0);
			break;
		}

		bool sentAny = false;
		for(NC* nc: _clusters) {
			if (!nc->nIn.isActive() || nc->_sent)
				continue;
			nc->_sent = true;
			sentAny = true;
			for (Link* link : nc->lUpOut) {
				link->_to.nIn += link->b;
			}
		}
		if (!sentAny)
			break;
	}
	// backward propagation
	for(NC* nc: _clusters) {
		if (!nc->nIn.isActive())
			continue;
		for (Link* link : nc->lUpIn) {
			link->_from.nIn += link->back;
		}
	}

	if (!isExceptionActive()) {
		if (_askLoadSensors->nIn.isActive() || _memorizeCurrentState->nIn.isActive())
			memorizeCurrentState();

		pushEffectors();
	}
	_activeChanged = false;
	bool cmdPresent = false;
	for(NC* nc: _clusters) {
		const bool act = nc->nIn.isActive();
		_activeChanged |= (act != nc->nPrev.isActive());
		cmdPresent |= (act && nc->_type == NC::TypeCmd);
	}
	_activeChanged |= cmdPresent; // _return command may leave with same activations but different hippo
	if (!_activeChanged && chainOfThoughtsMode())
		_return->nIn.activate();
}
void Brain::memorizeCurrentState() {
	if (!_hippo) {
		_hippo = newNC(NC::TypeHippo);
		_hippo->nIn.thr = 1;
	}
	// update links
	_hippo->lUpIn.deleteAllByStart();
	for(NC* nc: _clusters) {
		if (nc->nIn.isActive() && !nc->_isSystem)
			linkUp(nc, _hippo);
	}
	_hippo->nIn.thr = (float)_hippo->lUpIn.size();
	if (_hippo->lTIn.size() > 0)
		_hippo->nIn.thr += 1.f;
	if (0 == _hippo->nIn.thr)
		_hippo->nIn.thr = 1.f;

	if (_strictTuringMode->nIn.isActive()) {
		// in classic Turing machine, we only override current cell value without advance
	} else {
		// in biological implementation, new memory goes into it's own compartment, and we link it with older
		NC* newHip = newNC(NC::TypeHippo);
		linkTime(_hippo, newHip);
		_hippo = newHip;
	}
}
void Brain::callReturn() {
	assert(chainOfThoughtsMode());
	_activeChanged = true;
	if (_hippo) {
		for(NC* nc: _clusters) { // deactivate permanent neurons
			nc->nIn.act = 0;
			nc->nPrev.act = 0;
			nc->_sent = false;
		}
		_hippo = _hippo->prevNC();
	}
	if (!_hippo) {
		_askLoadSensors->nIn.activate();
		return;
	}
	for (Link* link : _hippo->lUpIn) {
		link->_from.nIn.activate();
	}
	_return->nPrev.activate();
}
void Brain::shiftLeft() {
	if (!_hippo)
		return;
	PNC left = _hippo->prevNC();
	if (!left) {
		_hippoLeftEnd->nIn.activate();
		const bool useInfiniteTape = _strictTuringMode->nIn.isActive();
		if (useInfiniteTape) {
			left = newNC(NC::TypeHippo);
			linkTime(left, _hippo);
		} else {
			return;
		}
	}
	_hippo = left;
	_activateMemory->nIn.activate();
}

void Brain::shiftRight() {
	if (!_hippo)
		return;
	PNC right = _hippo->lastNextNC();
	if (!right) {
		_hippoRightEnd->nIn.activate();
		const bool useInfiniteTape = _strictTuringMode->nIn.isActive();
		if (useInfiniteTape) {
			right = newNC(NC::TypeHippo);
			linkTime(_hippo, right);
		} else {
			return;
		}
	}
	_hippo = right;
	_activateMemory->nIn.activate();
}
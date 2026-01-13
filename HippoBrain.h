// HippoBrain.h
#pragma once
#include "SemanticNN.h"

class HippoBrain: public SemanticNN {
public:
	HippoBrain(std::initializer_list<char> types,
		std::initializer_list<float> thresholds,
		std::initializer_list<LinkInitializer> linksTime,
		std::initializer_list<LinkInitializer> linksUp
	);
	void inferenceCycle();
	void inferenceStep();
	void cycleUntillEnough();
protected:
	bool _activeChanged = true;
	PNC _askLoadSensors;
	PNC _return; // step hippocampus back + reset context to that step
	PNC _hippo; // current pointer to the active cluster that reveices memories or recalls them
	PNC _activateMemory; // recall memories from hippo
	PNC _shiftLeft;
	PNC _hippoLeftEnd;
	PNC _memorizeCurrentState;

	void finalizeCreation();
	bool isExceptionActive() const { return _return->nIn.isActive(); }
	bool chainOfThoughtsMode() const { return !_askLoadSensors->nIn.isActive(); }
	void shiftLeft();
	void callReturn();
	void memorizeCurrentState();
};
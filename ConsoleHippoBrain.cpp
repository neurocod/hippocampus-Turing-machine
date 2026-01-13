#include "ConsoleHippoBrain.h"
#include "Tokenizer.h"
#include <sstream>

ConsoleHippoBrain::ConsoleHippoBrain(std::initializer_list<char> types,
	std::initializer_list<float> thresholds,
	std::initializer_list<LinkInitializer> linksTime,
	std::initializer_list<LinkInitializer> linksUp) : HippoBrain(types, thresholds, linksTime, linksUp) {
}
void printHelp();
void ConsoleHippoBrain::loadSensors() {
	flushOutBuffer();
	if (_remainingInput.empty()) {
		std::cout << "[cycle:" << _cycle << "] >";
		std::string str;
		std::getline(std::cin, str);
		_remainingInput = Tokenizer::tokenizeReversed(str);
	}
	if (_remainingInput.empty()) {
		std::cout << "empty input, quit";
		_needExit = true;
		return;
	}
	++_tokensRead;
	_cycleForCurrentToken = 0;

	std::string token = _remainingInput.back();
	_remainingInput.pop_back();
	if (token == "quit") {
		_needExit = true;
		return;
	}
	if (token == "help") {
		printHelp();
		return loadSensors();
	}
	if (token == "repr") {
		printOutputEmbedding();
		return loadSensors();
	}

	NC* nc = 0;
	auto it = _sensors.find(token);
	if (it == _sensors.end()) {
		nc = newNC(NC::TypeSensor);
		_sensors[token] = nc->_id;
	} else {
		nc = byId(it->second);
	}
	if (nc)
		nc->nIn.activate();
}

void ConsoleHippoBrain::pushEffectors() {
	std::string out;
	for (auto [id, str] : _effectors) {
		PNC nc = byId(id);
		assert(id);
		if (nc->nIn.isActive()) {
			out += str;
		}
	}

	if (!out.empty()) {
		_outBuffer += out;
		if (_outBuffer.size() > _flushOutputThreshold)
			flushOutBuffer();
	}
}
void ConsoleHippoBrain::flushOutBuffer() {
	if (_outBuffer.empty())
		return;
	std::cout << _outBuffer;
	_outBuffer.clear();
}
void ConsoleHippoBrain::printOutputEmbedding()const {
	printActivations(outputEmbedding());
	writeActivationsToFile();
}
void ConsoleHippoBrain::printActivations(const std::vector<NC*>& list, size_t printColumnEvery, size_t neuronsPerRow)const {
	// prints '1' for active neurons and '.' otherwise
	if (list.empty()) {
		std::cout << "[]\n";
		return;
	}

	const size_t total = list.size();
	std::string formatted;
	for (size_t line = 0; line != -1; ++line) {
		if (line == 0)
			formatted.append("[");
		else
			formatted.append("\n ");

		for (size_t col = 0; col < neuronsPerRow; ++col) {
			const size_t i = line * neuronsPerRow + col;
			if (i >= total) {
				line = -1;
				break;
			}
			
			if (col && 0 == col % printColumnEvery)
				formatted.push_back('|');

			const NC* nc = list[i];
			if (!nc) {
				formatted.append("?");
				continue;
			}
			const float act = nc->nIn.act;

			if (nc->nIn.isActive())
				formatted.push_back('1');
			else
				formatted.push_back('.');
		}
		if (line == -1)
			break;
	}
	formatted.append("]\n");
	std::cout << formatted;
}
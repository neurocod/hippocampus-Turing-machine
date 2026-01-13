#pragma once
#include "HippoBrain.h"

class ConsoleHippoBrain : public HippoBrain {
public:
	ConsoleHippoBrain(std::initializer_list<char> types,
		std::initializer_list<float> thresholds,
		std::initializer_list<LinkInitializer> linksTime,
		std::initializer_list<LinkInitializer> linksUp
	);

	std::vector<std::string> _remainingInput; // reversed order, pop_back is used to read
	size_t _flushOutputThreshold = 100;

	void printOutputEmbedding()const;
	// 13 is 6 black chess pieces + 6 white + empty cell
	void printActivations(const std::vector<NC*>& list, size_t printColumnEvery = 13, size_t neuronsPerRow = 13*8)const;
protected:
	void loadSensors() override; // std::cin -> neurons
	void pushEffectors() override; // neurons -> std::cout

	std::map<std::string, bioId> _sensors;
	std::map<bioId, std::string> _effectors;
	std::string _outBuffer;

	void flushOutBuffer();
};
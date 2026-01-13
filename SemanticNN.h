//SemanticNN.h
#pragma once
#include "NeuroCluster.h"
#include <iostream>
#include <map>
#include <string>

class SemanticNN {
public:
	SemanticNN(std::initializer_list<char> types,
		std::initializer_list<float> thresholds,
		std::initializer_list<LinkInitializer> linksTime,
		std::initializer_list<LinkInitializer> linksUp
	);
	virtual ~SemanticNN();
	NC* newNC(NC::Type type);
	NC* byId(bioId id)const;
	bool removeNC(NC* nc);

	Link* linkTime(PNC from, PNC to);
	Link* linkUp(PNC from, PNC to);

	void writeActivationsToFile() const;
	virtual void loadSensors() = 0; // input -> neurons
	virtual void pushEffectors() = 0; // neurons -> std::cout
	virtual std::vector<NC*> outputEmbedding()const { return {}; }
protected:
	bioId _nextFreeId = 0;
	Vector<NC*> _clusters;
	uint64_t _tokensRead = 0;
	uint64_t _cycle = 0; // total
	uint64_t _cycleForCurrentToken = 0; // becomes 0 at each new token
	bool _needExit = false;
};
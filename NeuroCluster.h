#pragma once
#include "Link.h"

// Neuron body only, links are consolidated by NeuroCluster
// Neuron where activation is normalized to [0..1]
struct NeuronA1 {
	float act = 0; // activation
	bool isActive() const { return act >= .999f; }
	void activate() { act = 1.f; }
	void clipToThreshold() { if (act > 1.f) act = 1.f; }
	void operator+=(float f) { act += f; }
	void operator=(float f) { act = f; }
};

// Neuron where activation is normalized to [0..T]
struct NeuronAT {
	float act = 0; // activation
	float thr = 1; // threshold
	bool isActive() const { return act >= thr; }
	void activate() { act = thr; }
	void clipToThreshold() { if (act > thr) act = thr; }
	void operator+=(float f) { act += f; }
	void operator=(float f) { act = f; }
};

class SemanticNN;

// Neuro Cluster
class NC {
public:
	enum Type {
		TypeSensor,
		TypePermanent, // the only type that is processed differently (like LSTM);
		TypeTemp, // other types are just debug clarification
		TypeCmd,
		TypeHippo,
		TypeEffector,

		TypeLast
	};
	const bioId _id;
	const NC::Type _type;
	SemanticNN& _brain;
	NC(NC::Type type, bioId id, SemanticNN& brain);
	~NC();
	NC* prevNC()const;

	NeuronAT nIn; // input neuron for current step
	NeuronA1 nPrev; // input from previous time steps
	// output is calculated dynamically and not needed to be stored in some neuron

	LinksO lTOut; // time output link
	LinksI lTIn; // time input links
	LinksO lUpOut; // hierarchy out
	LinksI lUpIn; // hierarchy in

	bool _sent = false; // optimization
	bool _isSystem = false; // prevents memorizing by hippocampus if true
};

// Lightweight smart pointer to NC
class PNC {
protected:
	NC* p = 0;
public:
	PNC() {}
	PNC(nullptr_t) {}
	PNC(int null) {
		assert(0 == null);
	}
	PNC(const PNC& other) : p(other.p) {}
	PNC(const PNC* other) : p(other ? other->p : 0) {}
	PNC(NC* cluster) : p(cluster) {}
	PNC(NC& cluster) : p(&cluster) {}
	NC* operator=(NC& nc) {
		p = &nc;
		return p;
	}
	NC* operator->()const { return p; }
	operator NC* ()const { return p; }
	operator NC& ()const { return *p; }
	bool operator!=(const PNC& other)const { return p != other.p; }
	bool operator!=(const NC* other)const { return p != other; }
	bool operator!=(const NC& other)const { return p != &other; }

	bool operator!=(PNC& other)const { return p != other.p; }
	bool operator!=(NC* other)const { return p != other; }
	bool operator!=(NC& other)const { return p != &other; }
	
	bool operator==(const PNC& other)const { return p == other.p; }
	bool operator==(const NC& other)const { return p == &other; }
	bool operator==(const NC* other)const { return p == other; }
	bool operator==(bioId id)const {
		if (p)
			return p->_id == id;
		return id == 0;
	}
	bool operator==(PNC& other)const { return p == other.p; }
	bool operator==(NC& other)const { return p == &other; }
	bool operator==(NC* other)const { return p == other; }
};
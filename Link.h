#pragma once
#include "Vector.h"

class Link;
class NC; // NeuroCluster
using bioId = uint32_t; // cluster id

// link containers, input/output of the cluster
struct LinksI: public Vector<Link*> {};
struct LinksO: public Vector<Link*> {};

class Link {
public:
	NC& _from;
	NC& _to;
	LinksO& _lFrom;
	LinksI& _lTo;
	float b = 1; // bias
	float back = 0; // this signal is sent backwards from activated cluster to source
	
	Link(NC& from, NC& to, LinksO& lFrom, LinksI& lTo);
	~Link();
};

struct LinkInitializer { bioId from; bioId to; float b; float back; };
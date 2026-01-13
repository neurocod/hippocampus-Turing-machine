#include "Link.h"

bool operator!=(const NC& a, const NC& b) {
	return &a != &b;
}
Link::Link(NC& from, NC& to, LinksO& lFrom, LinksI& lTo):
	_from(from),
	_to(to),
	_lFrom(lFrom),
	_lTo(lTo)
{
#ifdef _DEBUG
	// search for duplicates
	for (Link* link : lFrom) {
		assert(link->_to != _to);
	}
	for (Link* link : lTo) {
		assert(link->_from != _from);
	}
#endif
	_lFrom.push_back(this);
	_lTo.push_back(this);
}
Link::~Link() {
	bool removed = _lFrom.removeOne(this);
	assert(removed);
	
	removed = _lTo.removeOne(this);
	assert(removed);
}
#include "NeuroCluster.h"
#include "SemanticNN.h"

NC::NC(NC::Type type, bioId id, SemanticNN& brain):
	_type(type), _id(id), _brain(brain)
{
}
NC::~NC() {
	lTOut.deleteAllByStart();
	lTIn.deleteAllByStart();
	lUpOut.deleteAllByStart();
	lUpIn.deleteAllByStart();
	_brain.removeNC(this);
}
NC* NC::prevNC()const {
	if (lTIn.empty())
		return nullptr;
	Link* link = *lTIn.begin();
	return &link->_from;
}
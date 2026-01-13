//SemanticNN.cpp
#include "SemanticNN.h"
#include <filesystem>
#include <fstream>
using namespace std;

using Brain = SemanticNN;

SemanticNN::SemanticNN(std::initializer_list<char> types,
	std::initializer_list<float> thresholds,
	std::initializer_list<LinkInitializer> linksTime,
	std::initializer_list<LinkInitializer> linksUp)
{
	assert(types.size() == thresholds.size());
	auto it = thresholds.begin();
	for (char type : types) {
		newNC((NC::Type)type)->nIn.thr = *it;
		++it;
	}
	for (auto&& init : linksTime) {
		PNC from = byId(init.from);
		PNC to = byId(init.to);
		assert(from && to);
		Link* link = linkTime(from, to);
		link->b = init.b;
		link->back = init.back;
	}
	for (auto&& init : linksUp) {
		PNC from = byId(init.from);
		PNC to = byId(init.to);
		assert(from && to);
		Link* link = linkUp(from, to);
		link->b = init.b;
		link->back = init.back;
	}
}
SemanticNN::~SemanticNN() {
	_clusters.deleteAllByEnd();
}
NC* Brain::newNC(NC::Type type) {
	// production should use optimized version like intrusive containers (instead of operator new)
	NC* ret = new NC(type, _nextFreeId++, *this);
	_clusters.push_back(ret);

	return ret;
}
bool Brain::removeNC(NC* nc) {
	if (!nc)
		return false;
	assert(this == &nc->_brain && !_clusters.empty());

	// Fast path: last element
	if (_clusters.back() == nc) {
		_clusters.pop_back();
		return true;
	}

	// Binary search
	auto it = std::lower_bound(_clusters.begin(), _clusters.end(), nc->_id,
		[](NC* a, bioId v) { return a->_id < v; });

	if (it != _clusters.end() && (*it)->_id == nc->_id) {
		assert(*it == nc);
		_clusters.erase(it);
		return true;
	}

	assert(false);
	// Fallback: linear search (should be uncommon)
	auto found = std::find(_clusters.begin(), _clusters.end(), nc);
	if (found == _clusters.end())
		return false;
	_clusters.erase(found);
	return true;
}
NC* Brain::byId(bioId id)const {
	if (id >= _nextFreeId || _clusters.empty())
		return nullptr;
	// optimization for when id's go as contiguous block of ids
	if (id < _clusters.size() && _clusters[id]->_id == id)
		return _clusters[id];
	// binary search on already sorted _clusters to find cluster with proper _id
	auto it = std::lower_bound(_clusters.begin(), _clusters.end(), id,
		[](NC* a, bioId v) { return a->_id < v; });
	if (it != _clusters.end() && (*it)->_id == id)
		return *it;
	return 0;
}
Link* Brain::linkTime(PNC from, PNC to) {
	return new Link(from, to, from->lTOut, to->lTIn);
}
Link* Brain::linkUp(PNC from, PNC to) {
	return new Link(from, to, from->lUpOut, to->lUpIn);
}
void Brain::writeActivationsToFile()const {
	const std::string filepath = R"(c:\g\temp\activations.bin)";

	// Ensure output directory exists (no-op if parent path is empty)
	std::filesystem::path p(filepath);
	if (!p.parent_path().empty()) {
		std::error_code ec;
		std::filesystem::create_directories(p.parent_path(), ec);
		// ignore errors; we'll detect failure when opening the file
	}

	std::ofstream ofs(filepath, std::ios::binary);
	if (!ofs) {
		std::cerr << "Failed to open file for writing: " << filepath << '\n';
		return;
	}

	// Write number of clusters as uint32_t
	uint32_t count = static_cast<uint32_t>(_clusters.size());
	ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
	for (NC* nc : _clusters) {
		char t = static_cast<char>(nc->_type);
		float v = nc->nIn.act;
		ofs.write(&t, sizeof(t));
		ofs.write(reinterpret_cast<const char*>(&v), sizeof(v));
	}

	ofs.close();
}
#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>
#include <memory>

template<class T>
class Vector : public std::vector<T> {
public:
	bool removeOne(T t) {
		auto it = std::find(this->begin(), this->end(), t);
		if (it == this->end())
			return false;
		this->erase(it);
		return true;
	}
	void deleteAllByStart() {
		while (1) {
			auto it = this->begin();
			if (it == this->end())
				break;
			T t = *it;
			delete t;
		}
	}
	void deleteAllByEnd() {
		while (1) {
			auto it = this->rbegin();
			if (it == this->rend())
				break;
			T t = *it;
			delete t;
		}
	}
	bool contains(T t)const {
		return std::find(this->begin(), this->end(), t) != this->end();
	}
};
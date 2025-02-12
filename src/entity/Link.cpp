#include "Link.h"

MYAI_BEGIN

Link &Link::operator=(Link &&rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}
	id	   = rhs.id;
	weight = rhs.weight;
	return *this;
}

Link &Link::operator=(const Link &rhs) {
	if (&rhs == this) {
		return *this;
	}
	id	   = rhs.id;
	weight = rhs.weight;
	return *this;
}

size_t LinkList::eraseByLessStand(weight_t w) {
	if (this->empty()) {
		return 0;
	}

	size_t rm_n = 0;
	for (auto it = this->begin(); it != this->end(); ++it) {
		if (static_cast<Link>(*it).weight > w) continue;
		this->erase(it);
		rm_n++;
	}
	return rm_n;
}
void LinkList::downWeight(weight_t w) {
	if (this->empty()) return;

	for (auto &[id, weight]: *this) {
		weight -= w;
	}
}
void LinkList::upWeight(weight_t w) {
	if (this->empty()) return;

	for (auto &[id, weight]: *this) {
		weight += w;
	}
}

inline weight_t LinkList::averageWeight() const {
	double sum = 0;
	for (auto &[id, weight]: *this) {
		sum += weight;
	}
	return sum / this->size();
}


LinkList::iterator LinkList::emplace(const nodeid_t &key, const weight_t &mapped) {
#if MYLIB_MSVC
	auto [_, bound] = _Find_lower_bound(key);
	if (_Lower_bound_duplicate(bound, mapped)) {
		bound->_Myval.second += mapped;
	} else {
		bound = _Emplace(key, mapped).first;
	}
	return iterator(bound, _Get_scary());
#elif MYLIB_GUN
	auto loc = lower_bound(key);
	if (loc == end() || key_comp()(key, loc->first)) {
		loc = emplace_hint(loc, key, mapped);
	} else {
		loc->second += mapped;
	}
	return iterator(loc._M_node);
#endif
}


MYAI_END
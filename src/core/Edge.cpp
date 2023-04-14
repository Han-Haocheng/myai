#include "Edge.h"


MYAI_BEGIN

Edge &Edge::operator=(Edge &&rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}
	id	   = rhs.id;
	weight = rhs.weight;
	return *this;
}

Edge &Edge::operator=(const Edge &rhs) {
	if (&rhs == this) {
		return *this;
	}
	id	   = rhs.id;
	weight = rhs.weight;
	return *this;
}

Edge &EdgeList::emplace(const value_type &val) {
	auto fd_rt = m_map.find(val.id);
	if (fd_rt != m_map.end()) {
		fd_rt->second.weight += val.weight;
		return fd_rt->second;
	}
	auto rt = m_map.emplace(val.id, val);
	return rt.first->second;
}

EdgeList::iterator EdgeList::find(const nodeid_t &key) {
	auto fd_rt = m_map.find(key);
	if (fd_rt == m_map.end()) {
		return m_map.end();
	};
	return m_map.find(key);
}

void EdgeList::insert(EdgeList::const_iterator first, EdgeList::const_iterator last) {
	for (auto it = first; it != last; ++it) {
		emplace(it->second);
	}
}

void EdgeList::insert(const EdgeList &list) {
	for (auto &[id, edge]: list.m_map) {
		emplace(edge);
	}
}

MYAI_END

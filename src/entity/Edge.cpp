#include "Edge.h"


MYAI_BEGIN

Edge::Edge(const nodeid_t i, const weight_t w) : id(i), weight(w) {}

Edge &EdgeList::emplace(const value_type &val) {
	auto rt = m_map.try_emplace(val.id, val);
	if (!rt.second) {
		rt.first->second.weight += val.weight;
	}
	return rt.first->second;
}

EdgeList::iterator EdgeList::find(const nodeid_t &key) {
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

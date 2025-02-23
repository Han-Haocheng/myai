#include "MyaiNode.h"

#include <memory>
#include <sys/stat.h>
#include <utility>

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

Edge &EdgeList::emplace(const nodeid_t &key, const weight_t &mapped) {
	auto fd_rt = m_map.find(key);
	if (fd_rt != m_map.end()) {
		fd_rt->second.weight += mapped;
		return fd_rt->second;
	}
	m_links.emplace_back(key, mapped);
	m_map.emplace(key, m_links.back());
	return m_links.back();
}


void MyaiNode::serialize(std::ostream &out) const {
	out << m_id << m_bias << m_state;
	out << m_links.size();
	for (const auto &lk: m_links) {
		out << lk.id << lk.weight;
	}
}

void MyaiNode::deserialize(std::istream &in) {
	in >> m_id;
	in >> m_bias;
	in >> *reinterpret_cast<enum_size *>(&m_state);
	size_t size;
	in >> size;
	weight_t w;
	nodeid_t id;
	for (size_t i = 0; i < size; ++i) {
		in >> id >> w;
		m_links.emplace(id, w);
	}
}

MYAI_END

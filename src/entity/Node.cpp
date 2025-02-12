

#include "Node.h"

#include <memory>
#include <sys/stat.h>
#include <utility>

MYAI_BEGIN
void Node::serialize(std::ostream &out) const {
	out << m_id << m_bias << m_state;
	out << m_links.size();
	for (const auto &[id, weight]: m_links) {
		out << id << weight;
	}
}

void Node::deserialize(std::istream &in) {
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

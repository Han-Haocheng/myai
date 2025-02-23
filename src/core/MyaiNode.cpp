#include "MyaiNode.h"

#include <sys/stat.h>

MYAI_BEGIN

void MyaiNode::serialize(std::ostream &out) const {
	out << m_id << m_bias << m_state;
	out << m_links.size();
	for (const auto &lk: m_links) {
		out.write(reinterpret_cast<const char *>(&lk.second), sizeof(lk.second));
	}
}

void MyaiNode::deserialize(std::istream &in) {
	in >> m_id;
	in >> m_bias;
	in >> *reinterpret_cast<enum_size *>(&m_state);
	size_t size;
	in >> size;
	Edge edge;
	for (size_t i = 0; i < size; ++i) {
		in.read(reinterpret_cast<char *>(&edge), sizeof(edge));
		m_links.emplace(edge);
	}
}

MYAI_END

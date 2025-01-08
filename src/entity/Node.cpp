

#include "Node.h"

#include <memory>
#include <sys/stat.h>
#include <utility>

MYAI_BEGIN
void Node::serialize(std::ostream &out) const {
	out.write(reinterpret_cast<const char *>(&m_head), sizeof(m_head));
	out << m_links.size();
	out.write(reinterpret_cast<const char *>(&m_links[0]), sizeof(Link) * MAX_LINK_NUMS);
}

void Node::deserialize(std::istream &in) {
	in.read(reinterpret_cast<char *>(&m_head), sizeof(m_head));
	size_t size;
	in >> size;
	m_links.resize(size);
	in.read(reinterpret_cast<char *>(&m_links[0]), sizeof(Link) * m_links.size());
}

MYAI_END

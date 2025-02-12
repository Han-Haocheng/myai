#include "MyaiNode.h"

<<<<<<< HEAD
<<<<<<< HEAD
#include <sys/stat.h>

MYAI_BEGIN

=======
#include <memory>
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
#include <sys/stat.h>

MYAI_BEGIN

<<<<<<< HEAD
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


>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
void MyaiNode::serialize(std::ostream &out) const {
	out << m_id << m_bias << m_state;
	out << m_links.size();
	for (const auto &lk: m_links) {
<<<<<<< HEAD
<<<<<<< HEAD
		out.write(reinterpret_cast<const char *>(&lk.second), sizeof(lk.second));
=======
		out << lk.id << lk.weight;
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
		out.write(reinterpret_cast<const char *>(&lk.second), sizeof(lk.second));
>>>>>>> cf44269 (2025年2月23日 16:12:23)
	}
}

void MyaiNode::deserialize(std::istream &in) {
	in >> m_id;
	in >> m_bias;
	in >> *reinterpret_cast<enum_size *>(&m_state);
	size_t size;
	in >> size;
<<<<<<< HEAD
<<<<<<< HEAD
	Edge edge;
	for (size_t i = 0; i < size; ++i) {
		in.read(reinterpret_cast<char *>(&edge), sizeof(edge));
		m_links.emplace(edge);
=======
	weight_t w;
	nodeid_t id;
	for (size_t i = 0; i < size; ++i) {
		in >> id >> w;
		m_links.emplace(id, w);
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	Edge edge;
	for (size_t i = 0; i < size; ++i) {
		in.read(reinterpret_cast<char *>(&edge), sizeof(edge));
		m_links.emplace(edge);
>>>>>>> cf44269 (2025年2月23日 16:12:23)
	}
}

MYAI_END

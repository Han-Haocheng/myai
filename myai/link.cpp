#include "link.h"

MYAI_BEGIN
Link::Link() : id(0), weight(0.0) {}
Link::Link(nodeid_t lk_node, weight_t lk_weight) : id(lk_node), weight(lk_weight) {}
Link::Link(const Link::pair_t &pair) : m_pair(pair) {}


MYAI_END

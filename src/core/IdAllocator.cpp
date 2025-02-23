//
// Created by HanHaocheng on 2024/12/14.
//
#include "IdAllocator.h"

MYAI_BEGIN

IdAllocator::IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris) : m_range(alloc_range), m_allocated(allocated), m_debris(debris) {
}


nodeid_t IdAllocator::allocate() {
	if (!m_debris.empty()) {
		nodeid_t id = m_debris.back();
		m_debris.pop_back();
		return id;
	}

	if (m_allocated == m_range.second) {
		return 0;
	}
	return ++m_allocated;
}

bool IdAllocator::deallocate(nodeid_t id) {
	if (id > m_range.second) {
		return false;
	}
	m_debris.push_back(id);
	return true;
}


MYAI_END

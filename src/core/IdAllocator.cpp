//
// Created by HanHaocheng on 2024/12/14.
//
#include "IdAllocator.h"

MYAI_BEGIN

<<<<<<< HEAD
<<<<<<< HEAD
IdAllocator::IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris)
	: m_range(alloc_range), m_allocated(allocated), m_debris(debris) {
=======
IdAllocator::IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris) : m_range(alloc_range), m_allocated(allocated), m_debris(debris) {
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
IdAllocator::IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris)
	: m_range(alloc_range), m_allocated(allocated), m_debris(debris) {
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
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

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
nodeid_t IdAllocator::allocate(size_t size) {
	nodeid_t id = m_allocated;
	m_allocated += size;
	return id;
}

<<<<<<< HEAD
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
bool IdAllocator::deallocate(nodeid_t id) {
	if (id > m_range.second) {
		return false;
	}
	m_debris.push_back(id);
	return true;
}


MYAI_END

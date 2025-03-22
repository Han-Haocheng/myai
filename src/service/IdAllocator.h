//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_SERVICE_H
#define MYAI_SLN_MYAI_SERVICE_H

#include "../MyaiDefine.h"

#include <algorithm>
#include <vector>

MYAI_BEGIN
class IdAllocator {
public:
	using ptr = std::shared_ptr<IdAllocator>;
	IdAllocator(nodeid_t beg, size_t size)
		: m_range(beg, beg + size), m_allocated(beg), m_debris() {
	}
	IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris);

	nodeid_t allocate();
	nodeid_t allocate(size_t size);

	bool deallocate(nodeid_t id);

	bool isAllocate(nodeid_t id) {
		return id >= m_range.first && id < m_range.second && id <= m_allocated &&
			   std::find(m_debris.begin(), m_debris.end(), id) == m_debris.end();
	}

private:
	std::pair<nodeid_t, nodeid_t> m_range;
	nodeid_t m_allocated;
	std::vector<nodeid_t> m_debris;
};

MYAI_END
#endif//MYAI_SLN_MYAI_SERVICE_H

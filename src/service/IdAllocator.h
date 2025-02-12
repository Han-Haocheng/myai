//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_SERVICE_H
#define MYAI_SLN_MYAI_SERVICE_H

#include "../define.h"
#include <vector>

MYAI_BEGIN
class IdAllocator {
public:
	using ptr = std::shared_ptr<IdAllocator>;

	IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range, nodeid_t allocated, std::vector<nodeid_t> debris)
		: m_range(alloc_range), m_allocated(allocated), m_debris(debris) {
	}
	nodeid_t allocate() {
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
	bool deallocate(nodeid_t id) {
		if (id > m_range.second) {
			return false;
		}
		m_debris.push_back(id);
		return true;
	}
	bool isAllocate(nodeid_t id) {
		return id <= m_allocated;
	}

private:
	std::pair<nodeid_t, nodeid_t> m_range;
	nodeid_t m_allocated;
	std::vector<nodeid_t> m_debris;
};

MYAI_END
#endif//MYAI_SLN_MYAI_SERVICE_H

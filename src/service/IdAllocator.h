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

	IdAllocator(std::pair<nodeid_t, nodeid_t> alloc_range,
							nodeid_t allocated, std::vector<nodeid_t> debris);
	nodeid_t allocate();
	bool deallocate(nodeid_t id);
	bool isAllocate(nodeid_t id);

private:
	std::pair<nodeid_t, nodeid_t> m_range;
	nodeid_t m_allocated;
	std::vector<nodeid_t> m_debris;
};

MYAI_END
#endif//MYAI_SLN_MYAI_SERVICE_H

#ifndef MYAI_DRIVER_MEMORY_H_
#define MYAI_DRIVER_MEMORY_H_

#include "Driver.h"

MYAI_BEGIN

class MemoryDriver : public MyaiDriver {
public:
	using ptr = std::shared_ptr<MemoryDriver>;

	MemoryDriver(nodeid_t begin, size_t driver_size) : MyaiDriver(Type::DT_MEMORY, begin, driver_size) {}

private:
	virtual void collect_data() override {}
	virtual void regeiste_controls() override {}
};

MYAI_END
#endif// !MYAI_DRIVER_MEMORY_H_

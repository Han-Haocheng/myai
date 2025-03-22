#ifndef MYAI_DRIVER_MEMORY_H_
#define MYAI_DRIVER_MEMORY_H_

#include "Driver.h"

MYAI_BEGIN

class MemoryDriver final : public MyaiDriver {
public:
	using ptr = std::shared_ptr<MemoryDriver>;

	MemoryDriver() : MyaiDriver(DT_MEMORY, NULL_ID, 0) {}


private:
	void collect_data() override {}
	void set_reflexes() override {}
};

MYAI_END
#endif// !MYAI_DRIVER_MEMORY_H_

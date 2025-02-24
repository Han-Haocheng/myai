#ifndef MYAI_DRIVER_STATUS_H_
#define MYAI_DRIVER_STATUS_H_

#include "Driver.h"

MYAI_BEGIN

class StatusDriver : public MyaiDriver {
	friend class DriverManager;

public:
	using ptr = std::shared_ptr<StatusDriver>;

	// 构造函数，初始化StatusDriver对象
	StatusDriver(nodeid_t begin, size_t size) : MyaiDriver(Type::DT_STATUS, begin, size) {}

private:
	virtual void collect_data() override;
	virtual void regeiste_controls() override;

	size_t m_normal_size = 3;
	weight_t m_positive;//正向权重
	weight_t m_negative;// 反向权重
	weight_t m_filter;	// 过滤权重
	std::vector<std::vector<weight_t>> m_driver_weight;
};

MYAI_END


#endif// !MYAI_DRIVER_STATUS_H_

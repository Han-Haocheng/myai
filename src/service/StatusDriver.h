#ifndef MYAI_DRIVER_STATUS_H_
#define MYAI_DRIVER_STATUS_H_

#include "Driver.h"

MYAI_BEGIN

class StatusDriver final : public MyaiDriver {
public:
	using ptr = std::shared_ptr<StatusDriver>;

	// 构造函数，初始化StatusDriver对象
	StatusDriver(nodeid_t begin, size_t size) : MyaiDriver(Type::DT_STATUS, begin, size) {}

	[[nodiscard]] auto positive() const { return m_positive; }
	[[nodiscard]] auto negative() const { return m_negative; }
	[[nodiscard]] auto filter() const { return m_filter; }
	[[nodiscard]] auto driver_weight(Type source, Type target) const { return m_driver_weight[source][target]; }
	[[nodiscard]] String show() const;

private:
	 void collect_data() override;
	 void set_reflexes() override;

	size_t m_normal_size = 3;
	weight_t m_positive{};//正向权重
	weight_t m_negative{};// 反向权重
	weight_t m_filter{};	// 过滤权重
	std::vector<std::vector<weight_t>> m_driver_weight;
};

MYAI_END


#endif// !MYAI_DRIVER_STATUS_H_

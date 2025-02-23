#ifndef MYAI_DRIVER_STATUS_H_
#define MYAI_DRIVER_STATUS_H_

#include "Driver.h"

MYAI_BEGIN

class StatusDriver : public MyaiDriver {
public:
	using ptr = std::shared_ptr<StatusDriver>;

	// 构造函数，初始化StatusDriver对象
	StatusDriver(nodeid_t begin, size_t size) : MyaiDriver(Type::DT_STATUS, begin, size) {}

	// 返回m_positive的值
	auto positive() const { return m_positive; }
	auto negative() const { return m_negative; }
	auto filter() const { return m_filter; }
	auto driver_weight(super::Type source, super::Type target) const { return m_driver_weight[source][target]; }

private:
	virtual void collect_data() override {


		for (size_t i = 0, j = 0; i < __DT_END__; ++i) {
			for (j = 0; j < __DT_END__; ++j) {
				m_collects->emplace(m_begin + i + j, m_driver_weight[i][j]);
			}
		}
		m_collects->emplace(m_begin + __DT_END__, m_positive);
		m_collects->emplace(m_begin + __DT_END__ + 1, m_negative);
		m_collects->emplace(m_begin + __DT_END__ + 2, m_filter);
	}

	// 重写regeiste_controls函数
	virtual void regeiste_controls() override {
		m_driver_weight.assign(__DT_END__, std::vector<weight_t>(__DT_END__, weight_t()));
		for (size_t i = 0, j = 0; i < __DT_END__; ++i)
			for (j = 0; j < __DT_END__; ++j)
				super::S_CONNECTIONS.emplace(m_begin + i + j, [&](weight_t w) { m_driver_weight[i][j] = w; });

		super::S_CONNECTIONS.emplace(m_begin + __DT_END__, [this](weight_t w) { m_positive = w; });
		super::S_CONNECTIONS.emplace(m_begin + __DT_END__ + 1, [this](weight_t w) { m_negative = w; });
		super::S_CONNECTIONS.emplace(m_begin + __DT_END__ + 2, [this](weight_t w) { m_filter = w; });
	}

	size_t m_normal_size = 3;
	weight_t m_positive;//正向权重
	weight_t m_negative;// 反向权重
	weight_t m_filter;	// 过滤权重
	std::vector<std::vector<weight_t>> m_driver_weight;
};

MYAI_END


#endif// !MYAI_DRIVER_STATUS_H_

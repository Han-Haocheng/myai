#ifndef MYAI_DRIVER_STATUS_H_
#define MYAI_DRIVER_STATUS_H_

#include "Driver.h"

MYAI_BEGIN


class StatusDriver : public MyaiDriver {
public:
	MYLIB_CONSTEXPR_TYPE MAX_STATUS_SIZE = 0x100;

	// 构造函数，初始化StatusDriver对象
	StatusDriver(nodeid_t begin, size_t driver_size) : MyaiDriver(Type::DT_STATUS, begin, 0x100), m_driver_size(driver_size) {}

	// 返回m_positive的值
	auto positive() const { return m_positive; }
	auto negative() const { return m_negative; }
	auto filter() const { return m_filter; }
	auto driver_weight(super::Type source, super::Type target) const { return m_driver_weight[source][target]; }

private:

	virtual void collect_data() override {
		m_collects->emplace(m_begin, m_positive);
		m_collects->emplace(m_begin + 1, m_negative);
		m_collects->emplace(m_begin + 2, m_filter);

		for (size_t i = 0, j = 0; i < m_driver_size; ++i) {
			for (j = 0; j < m_driver_size; ++j) {
				m_collects->emplace(m_begin + m_normal_size + i + j, m_driver_weight[i][j]);
			}
		}
	}

	// 重写regeiste_controls函数
	virtual void regeiste_controls() override {
		m_driver_weight.assign(m_driver_size, std::vector<weight_t>(m_driver_size, weight_t()));
		super::S_CONNECTIONS.emplace(m_begin, [this](weight_t w) { m_positive = w; });
		super::S_CONNECTIONS.emplace(m_begin + 1, [this](weight_t w) { m_negative = w; });
		super::S_CONNECTIONS.emplace(m_begin + 2, [this](weight_t w) { m_filter = w; });
		for (size_t i = 0, j = 0; i < m_driver_size; ++i) {
			for (j = 0; j < m_driver_size; ++j) {
				super::S_CONNECTIONS.emplace(m_begin + m_normal_size + i + j, [=](weight_t w) {
					m_driver_weight[i][j] = w;
				});
			}
		}
	}

	size_t m_driver_size;
	size_t m_normal_size = 3;
	weight_t m_positive;//正向权重
	weight_t m_negative; // 反向权重
	weight_t m_filter;	// 过滤权重
	std::vector<std::vector<weight_t>> m_driver_weight;
};

MYAI_END


#endif// !MYAI_DRIVER_STATUS_H_

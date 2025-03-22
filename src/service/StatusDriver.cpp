#include "StatusDriver.h"


MYAI_BEGIN

String StatusDriver::show() const {
	SString msg;
	for (size_t i = 0; i < __DT_END__; ++i)
		for (size_t j = 0; j < __DT_END__; ++j)
			msg << i << "\t" << j << "\t" << m_driver_weight[i][j] << "\n";
	return msg.str();
}

void StatusDriver::collect_data() {
	for (size_t i = 0, j = 0; i < __DT_END__; ++i)
		for (j = 0; j < __DT_END__; ++j)
			m_collects->emplace(m_begin + i + j, m_driver_weight[i][j]);

	m_collects->emplace(m_begin + __DT_END__ * __DT_END__ + 0, m_positive);
	m_collects->emplace(m_begin + __DT_END__ * __DT_END__ + 1, m_negative);
	m_collects->emplace(m_begin + __DT_END__ * __DT_END__ + 2, m_filter);
}

void StatusDriver::set_reflexes() {
	m_driver_weight.clear();
	for (size_t i = 0; i < __DT_END__; ++i)
		for (size_t j = 0; j < __DT_END__; ++j)
			m_reflexes.emplace_back(m_begin + i + j, [&](weight_t w) { m_driver_weight[i][j] = w; });
	m_reflexes.emplace_back(m_begin + __DT_END__ * __DT_END__ + 0, [&](weight_t w) { m_positive = w; });
	m_reflexes.emplace_back(m_begin + __DT_END__ * __DT_END__ + 1, [&](weight_t w) { m_negative = w; });
	m_reflexes.emplace_back(m_begin + __DT_END__ * __DT_END__ + 2, [&](weight_t w) { m_filter = w; });
}

MYAI_END

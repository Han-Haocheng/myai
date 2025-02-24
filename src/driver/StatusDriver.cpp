#include "StatusDriver.h"


MYAI_BEGIN

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
void StatusDriver::collect_data() {
	for (size_t i = 0, j = 0; i < __DT_END__; ++i) {
		for (j = 0; j < __DT_END__; ++j) {
			m_collects->emplace(m_begin + i + j, m_driver_weight[i][j]);
		}
	}
	m_collects->emplace(m_begin + __DT_END__, m_positive);
	m_collects->emplace(m_begin + __DT_END__ + 1, m_negative);
	m_collects->emplace(m_begin + __DT_END__ + 2, m_filter);
}

void StatusDriver::regeiste_controls() {
	m_driver_weight.assign(__DT_END__, std::vector<weight_t>(__DT_END__, weight_t()));
	for (size_t i = 0, j = 0; i < __DT_END__; ++i)
		for (j = 0; j < __DT_END__; ++j)
			super::S_CONNECTIONS.emplace(m_begin + i + j, [&](weight_t w) { m_driver_weight[i][j] = w; });

	super::S_CONNECTIONS.emplace(m_begin + __DT_END__, [this](weight_t w) { m_positive = w; });
	super::S_CONNECTIONS.emplace(m_begin + __DT_END__ + 1, [this](weight_t w) { m_negative = w; });
	super::S_CONNECTIONS.emplace(m_begin + __DT_END__ + 2, [this](weight_t w) { m_filter = w; });
}
<<<<<<< HEAD
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)

MYAI_END

#include "DriverManager.h"

MYAI_BEGIN

void DriverManager::collect(EdgeList::ptr out) {
	m_status = static_cast<Status>(COLLECT | BEGIN);
	for (auto &[name,var]: m_drivers) {
		var->collect(out);
	}
	m_status = static_cast<Status>(COLLECT | END);
}

void DriverManager::control(const Edge &output) {
	auto fd_rt = m_reflex.find(output.id);
	if (fd_rt == m_reflex.end()) {
		MYLIB_THROW("DriverManager::control: invalid connection id");
	}
	fd_rt->second(output.weight);
}
MYAI_END

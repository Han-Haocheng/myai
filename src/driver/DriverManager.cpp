#include "DriverManager.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)

MYAI_BEGIN

MyaiDriver::ptr DriverManager::addDriver(MyaiDriver::ptr driver) {
	m_drivers.push_back(driver);
	return driver;
}

void DriverManager::collect(EdgeList::ptr out) {
	for (auto &var: m_drivers) {
		auto temp = var->collect();
		out->insert(temp->begin(), temp->end());
	}
}

void DriverManager::control(const Edge &output) {
	MyaiDriver::S_CONNECTIONS.at(output.id)(output.weight);
}

<<<<<<< HEAD
MYAI_END
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
MYAI_END
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)

#ifndef MYLIB_DRIVER_MANAGER_H_
#define MYLIB_DRIVER_MANAGER_H_


#include "Driver.h"
#include "StatusDriver.h"


MYAI_BEGIN

class DriverManager {
public:
	using ptr = std::shared_ptr<DriverManager>;
	// using Motive = void(const Edge &);
	static constexpr nodeid_t MAX_CONTROL_NODE_ID = 0x1000'0000;

	MyaiDriver::ptr addDriver(MyaiDriver::ptr driver) {
		m_drivers.push_back(driver);
		return driver;
	}

	void collect(EdgeList::ptr out) {
		for (auto &var: m_drivers) {
			auto temp = var->collect();
			out->insert(temp->begin(), temp->end());
		}
	}

	/// @brief 控制
	/// @param output 对外输出数据
	void control(const Edge &output) {
		MyaiDriver::S_CONNECTIONS.at(output.id)(output.weight);
	}

private:
	std::vector<MyaiDriver::ptr> m_drivers;
};

MYAI_END

#endif// !MYLIB_DRIVER_MANAGER_H_

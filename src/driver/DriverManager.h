#ifndef MYLIB_DRIVER_MANAGER_H_
#define MYLIB_DRIVER_MANAGER_H_


#include "Driver.h"

MYAI_BEGIN

class DriverManager {
public:
	using ptr = std::shared_ptr<DriverManager>;
	// using Motive = void(const Edge &);

	bool loadConfig();
	bool saveConfig();

	MyaiDriver::ptr addDriver(MyaiDriver::ptr driver) {
		m_drivers.push_back(driver);
		return driver;
	}

	EdgeList &&collect() {
		EdgeList res;
		for (auto &var: m_drivers) {
			auto temp = var->collect();
			res.emplace(0, 0);
		}
		return std::move(res);
	}
	void control(const Edge &output) {
	}


	/// @brief 控制
	/// @param output 对外输出数据
	void control(EdgeList &output) {
		for (auto &var: output) {
			MyaiDriver::S_CONNECTIONS.at(var.id)(var.weight);
		}
	}

private:
	std::vector<MyaiDriver::ptr> m_drivers;
};

MYAI_END

#endif// !MYLIB_DRIVER_MANAGER_H_

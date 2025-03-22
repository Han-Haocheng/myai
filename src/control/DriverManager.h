#ifndef MYLIB_DRIVER_MANAGER_H_
#define MYLIB_DRIVER_MANAGER_H_


#include <utility>

#include "../service/Driver.h"
#include "../service/MemoryDriver.h"
#include "../service/StatusDriver.h"

#include "../service/MyaiService.h"


MYAI_BEGIN

struct DriverConfig {};


class DriverManager {
public:
	enum Status : uint16 {
		END		   = 0x0,
		BEGIN	   = 0x1,
		CREATE	   = 0x2,
		INITIALIZE = 0x4,
		COLLECT	   = 0x8,
		CONTROL	   = 0x10,
	};
	using ptr = std::shared_ptr<DriverManager>;
	// using Motive = void(const Edge &);
	static constexpr nodeid_t MAX_CONTROL_NODE_ID = 0x1000'0000;


	explicit DriverManager(MyaiService::ptr ser)
		: m_service(std::move(ser)) {
		m_status = static_cast<Status>(CREATE | BEGIN);

		m_status = static_cast<Status>(CREATE | END);
	}

	void init() {
		if (m_status != END) {
			MYLIB_THROW("running error: The status of current is not end.");
		}
		m_status = static_cast<Status>(INITIALIZE | BEGIN);

		m_status = static_cast<Status>(INITIALIZE | END);
	}

	template<class DriverTy, class... ARGS>
	typename DriverTy::ptr emplace(String &&name, ARGS &&...args) {
		typename DriverTy::ptr driver = std::make_shared<DriverTy>(std::bind(args)...);
		m_drivers.emplace(std::move(name), driver);
		driver.init();
		driver.get();
		return driver;
	}

	template<class DriverType>
	typename DriverType::ptr getDriver(String driver){
		return std::dynamic_pointer_cast<DriverType>(m_drivers[driver]);
	}

	void collect(EdgeList::ptr out);

	/// @brief 控制
	/// @param output 对外输出数据
	void control(const Edge &output);

	// 返回m_positive的值
private:
	Status m_status;
	MyaiService::ptr m_service;
	StatusDriver::ptr m_myai_status;
	MemoryDriver::ptr m_memory;
	//std::vector<MyaiDriver::ptr> m_drivers;
	std::unordered_map<String, MyaiDriver::ptr> m_drivers;
	std::unordered_map<nodeid_t, MyaiDriver::Reflex> m_reflex;
};

MYAI_END

#endif// !MYLIB_DRIVER_MANAGER_H_

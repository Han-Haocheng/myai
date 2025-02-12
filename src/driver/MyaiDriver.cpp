#include "MyaiDriver.h"

MYAI_BEGIN

std::map<MyaiDriver::Type, MyaiDriver::DriverFactory> MyaiDriver::S_FACTORIES = {};

MyaiDriver::MyaiDriver(Type type, nodeid_t begin, size_t id_size)
		: m_type(type), m_begin(begin), m_id_size(id_size) {
	if (S_FACTORIES.empty()) {
		S_FACTORIES[DT_EMOTION] = [](nodeid_t) -> MyaiDriver::ptr {return nullptr;};
		//todo: 创建驱动工厂函数映射
	}
}

MYAI_END
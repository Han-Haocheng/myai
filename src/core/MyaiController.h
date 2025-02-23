//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_CONTROL_H
#define MYAI_SLN_MYAI_CONTROL_H


#include "../driver/DriverManager.h"


#include "MyaiService.h"


#include <mylib/config/ConfigManager.h>


MYAI_BEGIN
class MyaiConfig {

public:
	using ptr	 = std::shared_ptr<MyaiConfig>;
	MyaiConfig() = default;
};

//=================================================================
// 控制层
//=================================================================

class MyaiController {
public:
	explicit MyaiController(size_t reasoning_max = 10)
		: m_reasoning_size(0), m_reasoning_max(reasoning_max), m_status() {
		init();
	}
	~MyaiController(){
		destroy();
	}

	void init() {}
	void destroy() {}
	void stop() {}

	void run();

	void reasoningCycle();
	void trainingCycle();

private:
	weight_t func(weight_t x) {
		return log2f(x + 1);
	}

private:
	size_t m_reasoning_size;
	size_t m_reasoning_max;
	weight_t m_focus;

	MyaiStatus m_status;

	MyaiConfig::ptr m_myai_config;

	MyaiService::ptr m_service;
	DriverManager::ptr m_driver_manager;
};

MYAI_END
#endif//MYAI_SLN_MYAI_CONTROL_H

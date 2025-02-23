//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_CONTROL_H
#define MYAI_SLN_MYAI_CONTROL_H


#include "MyaiService.h"

#include "../driver/DriverManager.h"


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
		: m_reasoning_size(0), m_reasoning_max(reasoning_max) {
	}
	~MyaiController() {
	}

	void init() {}
	void destroy() {}
	void stop() {}

	void run();

	void reasoningCycle();
	void trainingCycle();

private:
	weight_t func(weight_t x) {
		return x > 0 ? log2f(x + 1) : x * 0.01;
	}

private:
	struct TempInfo {
		MyaiNode::ptr node;
		weight_t attach_weight;
	};
	size_t m_reasoning_size;
	size_t m_reasoning_max;

	MyaiConfig::ptr m_myai_config;

	MyaiService::ptr m_service;
	DriverManager::ptr m_driver_manager;
	std::vector<TempInfo> m_temp_nodes;
};

MYAI_END
#endif//MYAI_SLN_MYAI_CONTROL_H

//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_CONTROL_H
#define MYAI_SLN_MYAI_CONTROL_H


<<<<<<< HEAD
#include "MyaiService.h"

#include "../driver/DriverManager.h"


=======
#include "../driver/DriverManager.h"


#include "MyaiService.h"


>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
#include <mylib/config/ConfigManager.h>


MYAI_BEGIN
class MyaiConfig {
<<<<<<< HEAD
public:
	using ptr	 = std::shared_ptr<MyaiConfig>;
	MyaiConfig() = default;

private:
=======

public:
	using ptr	 = std::shared_ptr<MyaiConfig>;
	MyaiConfig() = default;
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
};

//=================================================================
// 控制层
//=================================================================

class MyaiController {
public:
<<<<<<< HEAD
<<<<<<< HEAD
	explicit MyaiController(size_t reasoning_max = 10)
		: m_reasoning_size(0), m_reasoning_max(reasoning_max) {
	}
	~MyaiController() {
	}

	void init();
=======
	explicit MyaiController(size_t reasoning_max);
	~MyaiController();
=======
	explicit MyaiController(size_t reasoning_max = 10)
		: m_reasoning_size(0), m_reasoning_max(reasoning_max), m_status() {
		init();
	}
	~MyaiController(){
		destroy();
	}

>>>>>>> cf44269 (2025年2月23日 16:12:23)
	void init() {}
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
	void destroy() {}
	void stop() {}

	void run();

	void reasoningCycle();
	void trainingCycle();

private:
	weight_t func(weight_t x) {
<<<<<<< HEAD
		return x > 0 ? log2f(x + 1) : x * 0.01;
	}

private:
	struct TempInfo {
		MyaiNode::ptr node;
		weight_t attach_weight;
		weight_t filter_weight;
	};
	size_t m_reasoning_size;
	size_t m_reasoning_max;

	MyaiDao::ptr m_dao;
	IdAllocator::ptr m_id_alloc;
	MyaiConfig::ptr m_config;
	MyaiService::ptr m_service;
	DriverManager::ptr m_driver_manager;

	std::vector<TempInfo> m_temp_nodes;
=======
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
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
};

MYAI_END
#endif//MYAI_SLN_MYAI_CONTROL_H

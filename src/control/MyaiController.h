//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_CONTROL_H
#define MYAI_SLN_MYAI_CONTROL_H

#include "../service/NodeService.h"
#include "../driver/MyaiDriverManager.h"
#include "../entity/Emotion.h"

#include <mylib/Configurator.h>


MYAI_BEGIN

//=================================================================
// 控制层
//=================================================================

class MyaiController {
public:
	enum Status {
		NCS_UNDEF,
		NCS_INIT,
		NCS_RUNNING,
		NCS_STOP,
		NCS_DESTROY,
	};

	explicit MyaiController(size_t reasoning_max);
	~MyaiController();
	void init(){ m_status = NCS_INIT;}
	void destroy(){m_status = NCS_DESTROY;}
	void stop(){m_status = NCS_STOP;}

	void run();

	void reasoningCycle();
	void trainingCycle();


private:
	size_t m_reasoning_size;
	size_t m_reasoning_max;
	Status m_status;

	Configurator::ptr m_myai_config;

	Emotion::ptr m_emo;
	NodeService::ptr m_service;
	MyaiDriverManager::ptr m_driver_manager;
};

MYAI_END
#endif//MYAI_SLN_MYAI_CONTROL_H

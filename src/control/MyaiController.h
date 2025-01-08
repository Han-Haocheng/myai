//
// Created by HanHaocheng on 2024/12/14.
//

#ifndef MYAI_SLN_MYAI_CONTROL_H
#define MYAI_SLN_MYAI_CONTROL_H

#include "../dao/LinkStream.h"
#include "../driver/MyaiDriverManager.h"
#include "../service/MyaiService.h"

MYAI_BEGIN

//=================================================================
// 控制层
//=================================================================

class MyaiController {
  public:
	enum Status {
		NCS_INIT,
		NCS_REASON,
		NCS_TRAIN,
		NCS_STOP,
		NCS_DESTROY,
	};

	void init();
	void stop();

	void run();

	void reasoningCycle() {
		Node::ptr node		   = m_service->createNode(0);
		LinkStream::ptr stream = std::make_shared<LinkStream>();
		//collect handler cheese

		m_driver_manager->control(stream);

		//m_service->activateNode();
	}

	void trainingCycle() {
	}


  private:
	weight_t m_emotion;
	weight_t m_focus;

	Status m_status;
	MyaiService::ptr m_service;

	MyaiDriverManager::ptr m_driver_manager;
};

MYAI_END
#endif//MYAI_SLN_MYAI_CONTROL_H

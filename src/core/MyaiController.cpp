//
// Created by HanHaocheng on 2024/12/14.
//
#include "MyaiController.h"

#include <thread>

MYAI_BEGIN

MyaiController::MyaiController(size_t reasoning_max)
	: m_reasoning_size(0), m_reasoning_max(reasoning_max), m_status() {
	init();
}

MyaiController::~MyaiController() {
	destroy();
}


void MyaiController::run() {
	while (m_reasoning_size < m_reasoning_max) {
		reasoningCycle();
		// 监视器
	}

	while (m_reasoning_size > 0) {
		trainingCycle();
		// 监视器
	}
}

void MyaiController::reasoningCycle() {
	++m_reasoning_size;
	//collect handler cheese

	const MyaiNode::ptr temp_node = m_service->createNode(m_focus);
	EdgeList collect		  = m_driver_manager->collect();

	for (auto &lk: collect) {
		lk.weight = func(lk.weight);
		m_driver_manager->control(lk);
	}

	temp_node->buffer() = collect;
}
void MyaiController::trainingCycle() {
	--m_reasoning_size;
}

MYAI_END

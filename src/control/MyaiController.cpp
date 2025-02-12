//
// Created by HanHaocheng on 2024/12/14.
//
#include "MyaiController.h"

#include <thread>

MYAI_BEGIN

MyaiController::MyaiController(size_t reasoning_max)
	: m_reasoning_size(0), m_reasoning_max(reasoning_max), m_status(NCS_UNDEF) {
	init();
}

MyaiController::~MyaiController() {
	destroy();
}


void MyaiController::run() {
	m_status = NCS_RUNNING;

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

	const Node::ptr temp_node = m_service->createNode(0.0);
	// temp_node->buffer() << m_driver_manager->collect();
	// m_driver_manager->control(temp_node->buffer());
}
void MyaiController::trainingCycle() {
	--m_reasoning_size;
}

MYAI_END

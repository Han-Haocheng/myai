//
// Created by HanHaocheng on 2024/12/14.
//
#include "MyaiController.h"


MYAI_BEGIN

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
	EdgeList::ptr collect		  = std::make_shared<EdgeList>();
	m_driver_manager->collect(collect);
	try {
		for (auto &[id, edge]: *collect) {
			edge.weight = func(edge.weight) - m_status->negative() + m_status->positive();
			if (edge.weight < m_status->filter()) {
				continue;
			}
			if (edge.id < DriverManager::MAX_CONTROL_NODE_ID) {
				m_driver_manager->control(edge);
				continue;
			}
		}
	} catch (std::out_of_range &e) {
	}
}
void MyaiController::trainingCycle() {
	--m_reasoning_size;
}

MYAI_END

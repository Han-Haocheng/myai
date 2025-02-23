//
// Created by HanHaocheng on 2024/12/14.
//
#include "MyaiController.h"


MYAI_BEGIN

void MyaiController::run() {
	while (m_reasoning_size < m_reasoning_max) {
		reasoningCycle();
		++m_reasoning_size;
		// 监视器
	}

	while (m_reasoning_size > 0) {
		trainingCycle();
		--m_reasoning_size;
		// 监视器
	}
}

void MyaiController::reasoningCycle() {
	const MyaiNode::ptr temp_node = m_service->createNode(m_driver_manager->filter());
	EdgeList::ptr collect		  = std::make_shared<EdgeList>();
	m_driver_manager->collect(collect);
	m_service->linkNode(m_temp_nodes.back().node, collect);

	for (auto &[id, edge]: *collect) {
		edge.weight = func(edge.weight) + m_temp_nodes.back().attach_weight;
		if (edge.weight < m_driver_manager->filter()) {
			continue;
		}
		if (edge.id < DriverManager::MAX_CONTROL_NODE_ID) {
			m_driver_manager->control(edge);
			continue;
		}
		m_driver_manager->activate_node(edge);
		m_service->linkNode(edge.id, Edge{temp_node->id(), edge.weight});
	}

	m_temp_nodes.emplace_back(temp_node, m_driver_manager->negative() + m_driver_manager->positive());
}
void MyaiController::trainingCycle() {
}

MYAI_END

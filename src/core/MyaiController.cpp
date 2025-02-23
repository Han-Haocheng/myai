//
// Created by HanHaocheng on 2024/12/14.
//
#include "MyaiController.h"

<<<<<<< HEAD
<<<<<<< HEAD

MYAI_BEGIN
void myai::MyaiController::init() {
	m_dao			 = std::make_shared<MyaiDao>("./data");
	m_id_alloc		 = std::make_shared<IdAllocator>(1, 100000);
	m_config		 = std::make_shared<MyaiConfig>();
	m_service		 = std::make_shared<MyaiService>(m_dao, m_id_alloc);
	m_driver_manager = std::make_shared<DriverManager>(m_service);
	m_driver_manager->init();
}

void MyaiController::run() {
	while (m_reasoning_size < m_reasoning_max) {
		reasoningCycle();
		++m_reasoning_size;
=======
#include <thread>
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)

MYAI_BEGIN

void MyaiController::run() {
	while (m_reasoning_size < m_reasoning_max) {
		reasoningCycle();
<<<<<<< HEAD
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
		++m_reasoning_size;
>>>>>>> 69dfd9f (2025年2月23日 17:16:44)
		// 监视器
	}

	while (m_reasoning_size > 0) {
		trainingCycle();
<<<<<<< HEAD
<<<<<<< HEAD
		--m_reasoning_size;
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
		--m_reasoning_size;
>>>>>>> 69dfd9f (2025年2月23日 17:16:44)
		// 监视器
	}
}

void MyaiController::reasoningCycle() {
<<<<<<< HEAD
<<<<<<< HEAD
	EdgeList::ptr collect = std::make_shared<EdgeList>();
	m_driver_manager->collect(collect);

	if (!m_temp_nodes.empty()) {
		m_service->linkNode(m_temp_nodes.back().node, collect);
	}
	weight_t attach_weight		  = m_driver_manager->negative() + m_driver_manager->positive();
	weight_t filter_weight		  = m_driver_manager->filter();
	const MyaiNode::ptr temp_node = m_service->createNode(filter_weight);

	for (auto &[id, edge]: *collect) {
		edge.weight = func(edge.weight) + attach_weight;
		if (edge.weight < filter_weight) {
			continue;
		}

		if (edge.id < DriverManager::MAX_CONTROL_NODE_ID) {
			m_driver_manager->control(edge);
			continue;
		}
		m_driver_manager->activate_node(edge);
		m_service->linkNode(edge.id, Edge{temp_node->id(), edge.weight});
	}

	m_temp_nodes.emplace_back(TempInfo{temp_node, attach_weight, filter_weight});
}
void MyaiController::trainingCycle() {
=======
	++m_reasoning_size;
	//collect handler cheese

	const MyaiNode::ptr temp_node = m_service->createNode(m_focus);
=======
	const MyaiNode::ptr temp_node = m_service->createNode(m_driver_manager->filter());
>>>>>>> 69dfd9f (2025年2月23日 17:16:44)
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

	m_temp_nodes.emplace_back(TempInfo{temp_node, m_driver_manager->negative() + m_driver_manager->positive()});
}
void MyaiController::trainingCycle() {
<<<<<<< HEAD
	--m_reasoning_size;
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> 69dfd9f (2025年2月23日 17:16:44)
}

MYAI_END

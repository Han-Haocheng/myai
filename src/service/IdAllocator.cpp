//
// Created by HanHaocheng on 2024/12/14.
//
#include "myai_service.h"

MYAI_BEGIN


inline Node::ptr MyaiService::createNode(weight_t bias) {
	nodeid_t id = 0;
	if (m_alloc->allocate(id)) {
		MYLIB_THROW("id alloc error");
	}

	//todo
	Node::ptr node = std::make_shared<Node>();
	if (!node) {
		m_alloc->deallocate(id);
		MYLIB_THROW("node is nullptr");
	}

	m_newNodeIds.emplace(node->id());
	m_buffer.emplace(node->id(), node);

	return node;
}

inline Node::ptr MyaiService::getNode(nodeid_t id) { return find_node(id); }

inline bool MyaiService::delNode(nodeid_t id) {
	if (m_alloc->deallocate(id)) {

		MYLIB_ASSERT(m_buffer.erase(id) > 0ULL || m_dao->deleteById(id) > 0, "id can deallocate but ");
	}
	return true;
}

inline bool MyaiService::addNodeLink(nodeid_t id, Link lk) {
	Node::ptr node = find_node(id);
	node->emplace(lk);
	return true;
}

inline bool MyaiService::saveAll() {
	for (auto &id: m_newNodeIds) {
		m_buffer.at(id);
	}
	m_buffer.clear();
	return true;
}

inline bool MyaiService::activateNode(NodeLinkStream::ptr out, nodeid_t id) {
	if (!out) {
		MYLIB_THROW("out is nullptr");
	}

	Node::ptr node = find_node(id);
	weight_t w = activate_func(node->m_sumActivate + node->m_bias);
	node->m_sumActivate = 0;
	for (auto &lk: node->m_links) {
		out->stream[lk.id] += w * lk.weight;
	}
	return true;
}

inline weight_t MyaiService::activate_func(weight_t weight) {
	return weight > 0 ? weight : 0;
}

inline Node::ptr MyaiService::find_node(nodeid_t id) {
	auto find_rt = m_buffer.find(id);
	Node::ptr res = find_rt == m_buffer.end() ? nullptr : find_rt->second;
	if (!res) {
		res = m_dao->selectById(id);
	}
	if (res) {
		m_buffer.emplace(id, res);
	}
	return res;
}

MYAI_END
#include "MyaiService.h"

MYAI_BEGIN

MyaiNode::ptr MyaiService::createNode(weight_t bias) {

	MyaiNode::ptr node = std::make_shared<MyaiNode>(m_alloc->allocate(), bias, MyaiNode::NDS_CREATE);
	m_updata_nodes.emplace(node->m_id, node);
	node->m_state = MyaiNode::NDS_READY;
	return node;
}

bool MyaiService::removeNodeById(nodeid_t _id) {
	const nodeid_t &id	 = _id;
	const auto fd_rt	 = m_updata_nodes.find(id);
	const MyaiNode::ptr node = fd_rt != m_updata_nodes.end() ? fd_rt->second : m_dao->selectById(id);

	if (node == nullptr) return false;
	if (node->m_state != MyaiNode::NDS_READY) MYLIB_THROW("node state is not ready");

	node->m_state = MyaiNode::NDS_DESTROY;
	m_alloc->deallocate(node->m_id);
	return true;
}

MYAI_END
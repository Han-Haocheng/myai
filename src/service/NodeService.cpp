#include "NodeService.h"

MYAI_BEGIN

Node::ptr NodeService::createNode(weight_t bias) {

	Node::ptr node = std::make_shared<Node>(m_alloc->allocate(), bias, Node::NDS_CREATE);
	m_updata_nodes.emplace(node->m_id, node);
	node->m_state = Node::NDS_READY;
	return node;
}

bool NodeService::removeNodeById(nodeid_t _id) {
	const nodeid_t &id	 = _id;
	const auto fd_rt	 = m_updata_nodes.find(id);
	const Node::ptr node = fd_rt != m_updata_nodes.end() ? fd_rt->second : m_dao->selectById(id);

	if (node == nullptr) return false;
	if (node->m_state != Node::NDS_READY) MYLIB_THROW("node state is not ready");

	node->m_state = Node::NDS_DESTROY;
	m_alloc->deallocate(node->m_id);
	return true;
}

MYAI_END
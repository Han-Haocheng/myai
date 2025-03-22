#include "MyaiService.h"

MYAI_BEGIN

MyaiNode::ptr MyaiService::createNode(weight_t bias) {

	MyaiNode::ptr node = std::make_shared<MyaiNode>(m_alloc->allocate(), bias, MyaiNode::NDS_CREATE);
	m_updata_nodes.emplace(node->m_id, node);
	node->m_state = MyaiNode::NDS_READY;
	return node;
}

bool MyaiService::removeNodeById(nodeid_t _id) {
	const nodeid_t &id		 = _id;
	const auto fd_rt		 = m_updata_nodes.find(id);
	const MyaiNode::ptr node = fd_rt != m_updata_nodes.end() ? fd_rt->second : m_dao->selectById(id);

	if (node == nullptr) return false;
	if (node->m_state != MyaiNode::NDS_READY) MYLIB_THROW("node state is not ready");

	node->m_state = MyaiNode::NDS_DESTROY;
	m_alloc->deallocate(node->m_id);
	return true;
}

MyaiNode::ptr MyaiService::getNodeById(nodeid_t id) {
	auto fd_rt = m_updata_nodes.find(id);
	if (fd_rt != m_updata_nodes.end()) {
		return fd_rt->second;
	}
	MyaiNode::ptr node = m_dao->selectById(id);
	return node;
}

//bool MyaiService::activatedNode(EdgeList::ptr out, Edge edge) {
//	MyaiNode::ptr node = getNodeById(edge.id);
//	if (node == nullptr)
//		return false;
//	node->for_each([&](Edge &link) {
//		out->emplace(Edge{link.id, link.weight * edge.weight});
//	});
//	return true;
//}

void MyaiService::linkNode(nodeid_t id, Edge link) {
	auto node = getNodeById(id);
	if (node == nullptr) {
		return;
	}
	node->buffer().emplace(link);
}

void MyaiService::linkNode(MyaiNode::ptr node, EdgeList::ptr links) {
	node->buffer().insert(links);
}

MYAI_END

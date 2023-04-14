#include "MyaiService.h"

MYAI_BEGIN

Node::ptr NodeService::createNode(weight_t bias) {
		Node::ptr node = std::make_shared<Node>(m_alloc->allocate(),bias,Node::NDS_CREATE);
		m_updata_nodes.emplace(node->m_head.id, node);
		node->m_head.state = Node::NDS_READY;
		return node;
}

bool NodeService::removeNodeById(nodeid_t id) {
		auto fd_rt = m_updata_nodes.find(id);
		Node::ptr node = nullptr;
		if (fd_rt != m_updata_nodes.end()) {
				node = fd_rt->second;
		}else {
				node = m_dao->selectById(id);
				if (node == nullptr) {
						MYLIB_THROW("node not found");
				}
		}
		if (node->m_head.state != Node::NDS_READY) {
				MYLIB_THROW("node state is not ready");
		}
		node->m_head.state = Node::NDS_DESTROY;
		m_alloc->deallocate(node->m_head.id);
		return true;
}



MYAI_END
#ifndef MYAI_SERVICE_NODESERVICE_H
#define MYAI_SERVICE_NODESERVICE_H

#include "../dao/MyaiDao.h"

#include "IdAllocator.h"

MYAI_BEGIN

/**
 * @brief 提供节点的控制和操作功能
 */
class NodeService {
public:
	using ptr = std::shared_ptr<NodeService>;

	// 创建节点
	Node::ptr createNode(weight_t bias);

	// 删除节点
	bool removeNodeById(nodeid_t id);

	// 获取节点
	Node::ptr getNodeById(nodeid_t id);

	// 修改节点


	// 激活节点
	bool activatedNode(LinkList &out, Node::ptr node, weight_t input);

private:
	std::unordered_map<nodeid_t, Node::ptr> m_updata_nodes;
	MyaiDao::ptr m_dao;
	IdAllocator::ptr m_alloc;
};

MYAI_END
#endif// !MYAI_SERVICE_NODESERVICE_H

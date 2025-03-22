#ifndef MYAI_SERVICE_NODESERVICE_H
#define MYAI_SERVICE_NODESERVICE_H

#include "../dao/MyaiDao.h"
#include "IdAllocator.h"
#include <unordered_map>

MYAI_BEGIN

/**
 * @brief 提供节点的控制和操作功能
 */
class MyaiService {
	friend class DriverManager;

public:
	using ptr = std::shared_ptr<MyaiService>;

	MyaiService(MyaiDao::ptr dao, IdAllocator::ptr id_alloc)
		: m_dao(dao), m_alloc(id_alloc) {
	}


	// 创建节点
	MyaiNode::ptr createNode(weight_t bias);

	// 删除节点
	bool removeNodeById(nodeid_t id);

	// 获取节点
	MyaiNode::ptr getNodeById(nodeid_t id);

	// bool activatedNode(EdgeList::ptr out, Edge edge);

	void linkNode(nodeid_t id, Edge link);
	void linkNode(MyaiNode::ptr node, EdgeList::ptr links);

	nodeid_t applyIds(size_t size) {
		return m_alloc->allocate(size);
	}

private:
	std::unordered_map<nodeid_t, MyaiNode::ptr> m_updata_nodes;
	MyaiDao::ptr m_dao;
	IdAllocator::ptr m_alloc;
};


MYAI_END
#endif// !MYAI_SERVICE_NODESERVICE_H

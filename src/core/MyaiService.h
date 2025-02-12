#ifndef MYAI_SERVICE_NODESERVICE_H
#define MYAI_SERVICE_NODESERVICE_H

#include "IdAllocator.h"
#include "MyaiDao.h"
<<<<<<< HEAD
=======

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
#include <unordered_map>

MYAI_BEGIN

/**
 * @brief 提供节点的控制和操作功能
 */
class MyaiService {
<<<<<<< HEAD
	friend class DriverManager;

public:
	using ptr = std::shared_ptr<MyaiService>;

	MyaiService(MyaiDao::ptr dao, IdAllocator::ptr id_alloc)
		: m_dao(dao), m_alloc(id_alloc) {
	}


=======
public:
	using ptr = std::shared_ptr<MyaiService>;

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
	// 创建节点
	MyaiNode::ptr createNode(weight_t bias);

	// 删除节点
	bool removeNodeById(nodeid_t id);

	// 获取节点
	MyaiNode::ptr getNodeById(nodeid_t id);

<<<<<<< HEAD
	bool activatedNode(EdgeList::ptr out, Edge edge);

	void linkNode(nodeid_t id, Edge link);
	void linkNode(MyaiNode::ptr node, EdgeList::ptr links);

private:
	nodeid_t applyId(size_t size) {
		return m_alloc->allocate(size);
	}
=======
	// 修改节点

	// 激活节点
	bool activatedNode(EdgeList &out, MyaiNode::ptr node, weight_t input);
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)

private:
	std::unordered_map<nodeid_t, MyaiNode::ptr> m_updata_nodes;
	MyaiDao::ptr m_dao;
	IdAllocator::ptr m_alloc;
};

MYAI_END
#endif// !MYAI_SERVICE_NODESERVICE_H

#ifndef MYAI_SERVICE_NODESERVICE_H
#define MYAI_SERVICE_NODESERVICE_H

#include "IdAllocator.h"
#include "MyaiDao.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> 23fcfe1 (2025年2月23日 20:57:23)
#include <unordered_map>

MYAI_BEGIN

/**
 * @brief 提供节点的控制和操作功能
 */
class MyaiService {
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	friend class DriverManager;

public:
	using ptr = std::shared_ptr<MyaiService>;

	MyaiService(MyaiDao::ptr dao, IdAllocator::ptr id_alloc)
		: m_dao(dao), m_alloc(id_alloc) {
	}


<<<<<<< HEAD
=======
public:
	using ptr = std::shared_ptr<MyaiService>;

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	// 创建节点
	MyaiNode::ptr createNode(weight_t bias);

	// 删除节点
	bool removeNodeById(nodeid_t id);

	// 获取节点
	MyaiNode::ptr getNodeById(nodeid_t id);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	// 激活节点
>>>>>>> 23fcfe1 (2025年2月23日 20:57:23)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	bool activatedNode(EdgeList::ptr out, Edge edge);

	void linkNode(nodeid_t id, Edge link);
	void linkNode(MyaiNode::ptr node, EdgeList::ptr links);

private:
	nodeid_t applyId(size_t size) {
		return m_alloc->allocate(size);
	}
<<<<<<< HEAD
=======
	// 修改节点

	// 激活节点
<<<<<<< HEAD
	bool activatedNode(EdgeList &out, MyaiNode::ptr node, weight_t input);
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	bool activatedNode(EdgeList::ptr out, Edge edge);

	void linkNode(nodeid_t id, Edge link);
	void linkNode(MyaiNode::ptr node, EdgeList::ptr links);
>>>>>>> 69dfd9f (2025年2月23日 17:16:44)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)

private:
	std::unordered_map<nodeid_t, MyaiNode::ptr> m_updata_nodes;
	MyaiDao::ptr m_dao;
	IdAllocator::ptr m_alloc;
};

MYAI_END
#endif// !MYAI_SERVICE_NODESERVICE_H

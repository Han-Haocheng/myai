#pragma once
#include "../dao/LinkStream.h"
#include "../dao/MyaiDao.h"

#include "IdAllocator.h"

MYAI_BEGIN

/**
 * @brief 提供节点的控制和操作功能
 */
class NodeService {
public:
  // 创建节点
  Node::ptr createNode();

  // 删除节点
  bool removeNodeById();

  // 获取节点
  Node::ptr getNodeById(nodeid_t id);
  std::vector<Node::ptr> getNodeById(const std::vector<nodeid_t> &ids);
  std::vector<Node::ptr> getNodeInBuffer();

  // 修改节点
  bool eraseLinkById(Node::ptr node, nodeid_t i);
  bool eraseLinkByLessStand(Node::ptr node, weight_t stand);
  bool pushLink(Node::ptr node, Link &&lk);
  bool pushLink(Node::ptr node, const Link &lk);
  bool pushLink(Node::ptr node, const std::vector<Link> &lks);
  bool clearLink(Node::ptr node);

private:
};


/**
 * @brief 提供整体的操作功能
 */
class MyaiService {
public:
  using ptr = std::shared_ptr<MyaiService>;

  MyaiService() = default;

  Node::ptr getNode(nodeid_t id);
  bool setNode(Node::ptr node);
  Node::ptr createNode(weight_t bias);
  bool deleteNode(nodeid_t id);

  
  // 执行一次推理周期
  bool activateNode(LinkStream::ptr output, nodeid_t id, weight_t actval);
  
  // 执行学习周期

  // 添加监视器

  // 添加驱动

private:
  weight_t activate_func(weight_t weight);
  Node::ptr find_node(nodeid_t id);

private:
  MyaiDao::ptr m_dao;
  IdAllocator::ptr m_alloc;
};

MYAI_END

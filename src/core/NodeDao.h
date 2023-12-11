//
// Created by HanHaocheng on 2023/11/29.
//

#ifndef THINK_NODEDAO_H
#define THINK_NODEDAO_H

#include "Node.h"
#include "NodeIdAllocator.h"
#include <unordered_map>
namespace myai
{

class NodeDao
{
public:
  using ptr = std::shared_ptr<NodeDao>;

  int insert(const Node::ptr &node);
  int update(Node::ptr node);
  int deleteById(uint64_t id);
  Node::ptr selectById(uint64_t id);

private:
  std::unordered_map<uint64_t, Node::ptr> m_nodeCache;
};

}// namespace myai

#endif//THINK_NODEDAO_H

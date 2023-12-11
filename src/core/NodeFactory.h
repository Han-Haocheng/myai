//
// Created by HanHaocheng on 2023/11/30.
//

#ifndef THINK_NODEFACTORY_H
#define THINK_NODEFACTORY_H

#include "Node.h"
#include "NodeIdAllocator.h"
namespace myai
{

class NodeFactory
{
public:
  using ptr = std::shared_ptr<NodeFactory>;

  NodeFactory(NodeIdAllocator::ptr idAllocator);
  Node::ptr getInstance(uint16_t bais, Node::Type type);

private:
  NodeIdAllocator::ptr m_idAllocator = nullptr;
};

}// namespace myai

#endif//THINK_NODEFACTORY_H

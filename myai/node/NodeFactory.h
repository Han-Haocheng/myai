//
// Created by HanHaocheng on 2024/2/19.
//

#ifndef MYAI_FACTORY_NODE_FACTORY_H_
#define MYAI_FACTORY_NODE_FACTORY_H_

#include "../config/define.h"
#include "NodeDao.h"
#include "Node.h"
#include "NodeIdAllocator.h"

MYAI_BEGIN

class NodeFactory {
  struct Info {
    //    id_t begin_id = -1;
    //    size_t block_num = 0;
    //    std::forward_list<id_t> ids;
    std::vector<Node::ptr> cache;
  };

public:
  using ptr = std::shared_ptr<NodeFactory>;



  NodeFactory(NodeDao::ptr dao, NodeIdAllocator::ptr allocator);
  ~NodeFactory() = default;
  Node::ptr createNode(Node::Type type, weight_t bias);
  Node::ptr createNode(id_t id);


  void clearBuffer();

  //void addBlock(Node::Type type, size_t block_num);
  //void setBlockSize(size_t block_size);

private:
  std::unordered_map<id_t, Node::ptr> m_buffer;


  NodeDao::ptr m_node_dao = nullptr;
  NodeIdAllocator::ptr m_allocator;
};

MYAI_END

#endif//MYAI_FACTORY_NODE_FACTORY_H_

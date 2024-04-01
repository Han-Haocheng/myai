#ifndef MYAI_NODE_DAO_H_
#define MYAI_NODE_DAO_H_


#include "myai_def.h"
#include "myai_id_allocator.h"
#include "myai_node.h"
#include <memory>

MYAI_SPACE_BEGIN
class MyaiNodeDao {
public:
  using ptr = std::shared_ptr<MyaiNodeDao>;

  MyaiNodeDao();
  ~MyaiNodeDao();

  Node::ptr createNode();

private:
  String m_data_root;
};


MYAI_SPACE_END

#endif// !MYAI_NODE_DAO_H_

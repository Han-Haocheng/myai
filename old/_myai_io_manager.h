#ifndef MYAI_IO_MANAGER_H_
#define MYAI_IO_MANAGER_H_

#include "../io/myai_io_interface.h"
#include "../io/myai_memory_io.h"
#include "myai_def.h"
#include "myai_node.h"
#include <functional>

MYAI_SPACE_BEGIN
class MyaiIOManager {
public:
  using ptr = std::shared_ptr<MyaiIOManager>;

  MyaiIOManager();
  ~MyaiIOManager();

  void init();

  void runIO(const Node::ptr &recover);

private:
  std::unordered_map<Node::Type, MyaiIOBase::ptr> m_ios;
  std::unordered_map<nid_t, Node::ptr> m_act_nodes;
  MyaiMemoryIO::ptr m_memory_io;
};

MYAI_SPACE_END
#endif// !MYAI_IO_MANAGER_H_

#ifndef MYAI_THINKER_H_
#define MYAI_THINKER_H_


#include "myai_def.h"
#include "myai_io_manager.h"
#include "myai_node.h"
#include "myai_node_dao.h"

MYAI_SPACE_BEGIN

class MyaiThinker {
public:
  using ptr = std::shared_ptr<MyaiThinker>;

  MyaiThinker(MyaiNodeDao::ptr node_dao, MyaiIOManager::ptr io_manager);
  ~MyaiThinker();

  void inferencePeriod();
  void studyPeriod();

private:
  MyaiNodeDao::ptr m_node_dao;
  MyaiIOManager::ptr m_io_manager;

  Node::ptr m_recover_node;
};

MYAI_SPACE_END
#endif// !MYAI_THINKER_H_

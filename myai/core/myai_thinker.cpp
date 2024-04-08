#include "myai_thinker.h"
#include "myai_node.h"

#include <utility>

MYAI_SPACE_BEGIN

MyaiThinker::MyaiThinker(MyaiNodeDao::ptr node_dao, MyaiIOManager::ptr io_manager)
    : m_io_manager(std::move(io_manager)), m_node_dao(std::move(node_dao)) {
}

MyaiThinker::~MyaiThinker() {
}

void MyaiThinker::inferencePeriod() {
  m_io_manager->runIO(m_recover_node);
  m_recover_node = m_node_dao->createNode();
  //m_io_manager->addRecoverNode(m_recover_node);
  //m_io_manager->output(m_recover_node);
}

void MyaiThinker::studyPeriod() {
}

MYAI_SPACE_END
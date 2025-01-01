
#include "myai_io_manager.h"


MYAI_SPACE_BEGIN


MyaiIOManager::MyaiIOManager() {
}

MyaiIOManager::~MyaiIOManager() {
}

void MyaiIOManager::runIO(const Node::ptr &recover) {
  for (const auto &myai_io: m_ios) {
    myai_io.second->output(m_act_nodes);
    myai_io.second->input(m_act_nodes);
    myai_io.second->addInputInfo({recover->id(), 50.0f});
  }
  for (const auto &item: m_act_nodes) {
    recover->links().lock()->emplace_back(item.first, item.second->activeValue);
  }
  m_act_nodes.clear();
}
MYAI_SPACE_END

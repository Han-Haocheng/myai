#include "myai_controller.h"

MYAI_SPACE_BEGIN
MyaiController::MyaiController(String configPath)
    : m_is_init(false),
      m_is_stop(true),
      m_is_stopping(false),
      m_config_path(configPath),
      m_factory(),
      m_id_allocator(),
      m_node_dao(),
      m_io_manager(),
      m_thinker(),
      m_thread_pool() {
  m_factory.reset(new MyaiFactory{configPath});
  m_factory->loadConfig();
  m_io_manager   = m_factory->createIOManager();
  m_id_allocator = m_factory->createIdAllocator();
  m_node_dao     = m_factory->createNodeDao();
  m_thinker      = m_factory->createThinker();
}

MyaiController::~MyaiController() {
  m_factory->saveConfig();
}

void MyaiController::init() {
}

void MyaiController::start() {
}

void MyaiController::stop() {
}

void MyaiController::isStop() {
}

void MyaiController::run() {
}


MYAI_SPACE_END

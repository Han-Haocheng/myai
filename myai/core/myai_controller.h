#ifndef MYAI_CONTROLLER_H_
#define MYAI_CONTROLLER_H_

#include "myai_def.h"
#include "myai_factory.h"
#include "myai_thinker.h"

#include <algorithm>
#include <memory>
#include <thread>
#include <vector>


MYAI_SPACE_BEGIN

class MyaiController {
public:
  MyaiController(String configPath = "config.json");

  ~MyaiController();


  void init();

  void start();
  void stop();

  void isStop();

private:
  void run();

private:
  boolean m_is_init : 1;
  boolean m_is_stop : 1;
  boolean m_is_stopping : 1;

  String m_config_path;

  MyaiFactory::ptr m_factory          = nullptr;
  MyaiIdAllocator::ptr m_id_allocator = nullptr;
  MyaiNodeDao::ptr m_node_dao         = nullptr;
  MyaiIOManager::ptr m_io_manager     = nullptr;
  MyaiThinker::ptr m_thinker          = nullptr;

  std::vector<std::shared_ptr<std::thread>>
          m_thread_pool = {};
};

MYAI_SPACE_END


#endif//MYAI_CONTROLLER_H_
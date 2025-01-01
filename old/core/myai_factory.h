#ifndef MYAI_FACTORY_H_


#define MYAI_FACTORY_H_
#include "myai_thinker.h"

MYAI_SPACE_BEGIN

class MyaiFactory {
public:
  using ptr = std::shared_ptr<MyaiFactory>;

  MyaiFactory(String config_file);
  ~MyaiFactory();
  void loadConfig();
  void saveConfig();


  MyaiIOManager::ptr createIOManager();
  MyaiIdAllocator::ptr createIdAllocator();
  MyaiNodeDao::ptr createNodeDao();
  MyaiThinker::ptr createThinker();

private:
  void load_from_json();
  void save_to_json();
  void load_from_yaml();
  void save_to_yaml();

private:
  String m_config_path;
};

MYAI_SPACE_END

#endif// !MYAI_FACTORY_H_

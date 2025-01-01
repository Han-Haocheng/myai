#include "myai_factory.h"
#include <algorithm>


MYAI_SPACE_BEGIN

MyaiFactory::MyaiFactory(String config_file)
    : m_config_path(config_file) {
}

MyaiFactory::~MyaiFactory() {
}
void MyaiFactory::loadConfig() {
  String file_type = m_config_path.substr(m_config_path.find_last_of("."));
  if (file_type.empty()) {
    return;
  }

  std::transform(file_type.begin(), file_type.end(), file_type.begin(), ::tolower);
  if (file_type == ".json") {
    load_from_json();
  } else if (file_type == ".yml" || file_type == ".yaml") {
    load_from_yaml();
  }
}
void MyaiFactory::saveConfig() {}

MyaiIOManager::ptr MyaiFactory::createIOManager() { return nullptr; }
MyaiIdAllocator::ptr MyaiFactory::createIdAllocator() { return nullptr; }
MyaiNodeDao::ptr MyaiFactory::createNodeDao() { return nullptr; }
MyaiThinker::ptr MyaiFactory::createThinker() { return nullptr; }



void MyaiFactory::load_from_json() {}
void MyaiFactory::save_to_json() {}
void MyaiFactory::load_from_yaml() {}
void MyaiFactory::save_to_yaml() {}

MYAI_SPACE_END
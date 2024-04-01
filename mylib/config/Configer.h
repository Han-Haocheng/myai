//
// Created by HanHaocheng on 2024/3/18.
//

#ifndef MY_AI_CONFIGER_H
#define MY_AI_CONFIGER_H
#include "../define/type.h"
#include "ConfigValue.h"
#include <unordered_map>

MYLIB_BEGIN
class Configer {
public:
  void load_from_yaml(String path);
  void save_to_yaml(String path);

  template<class ConfigTy>
  ConfigTy get_config(const String& key) {
    try{
      auto ptr = m_configs.at(key);
      return std::dynamic_pointer_cast<ConfigValue<ConfigTy>>();
    } catch (std::out_of_range&e) {
      
    }

  }

private:
  std::unordered_map<std::string, ConfigValueBase::ptr> m_configs;
};
MYLIB_END

#endif//MY_AI_CONFIGER_H

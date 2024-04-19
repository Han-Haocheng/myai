//
// Created by HanHaocheng on 2024/4/2.
//

#ifndef MY_PROJECT_CONFIG_MANAGER_H
#define MY_PROJECT_CONFIG_MANAGER_H
#include "../core/mylib_def.h"
#include "config_val.h"

#include <algorithm>
#include <unordered_map>
#include <utility>

MYLIB_SPACE_BEGIN

class ConfigManager {
public:
  using ptr = std::shared_ptr<ConfigManager>;

  explicit ConfigManager() : m_config_file_type(CFT_YAML), m_config_file("config.yml"), m_configs() {}
  explicit ConfigManager(String confPath) : m_config_file_type(CFT_UNKNOWN), m_config_file(std::move(confPath)), m_configs() {}
  ~ConfigManager() = default;

  void setConfigFile(const String &path);

  template<typename ConfTy>
  ConfTy &getConfig(const String &key);

  template<typename ConfTy>
  void setConfig(String key, ConfTy &&conf, String comment = "");

  void showAllConfigs();
  bool load();
  bool save();

private:
  void load_from_yaml();
  void load_from_json();
  void save_to_yaml();
  void save_to_json();
  ConfigValueBase::ptr get_config_base(const String &key);

private:
  enum ConfigFileType : int8
  {
    CFT_UNKNOWN = -1,
    CFT_YAML,
    CFT_JSON,
  };

  ConfigFileType m_config_file_type;
  String m_config_file;
  std::unordered_map<std::string, ConfigValueBase::ptr> m_configs;
};

template<typename ConfTy>
ConfTy &ConfigManager::getConfig(const String &key) {
  auto conf_val = get_config_base(key);
  if (!conf_val || conf_val->typeName() != typeid(ConfTy).name()) {
    return ConfTy{};
  }
  return *dynamic_cast<ConfTy *>(conf_val.get());
}

template<typename ConfTy>
void ConfigManager::setConfig(String key, ConfTy &&conf, String comment) {
  if (key.empty()) {
    return;
  }

  auto conf_val = get_config_base(key);
  if (conf_val) {
    conf_val.reset(new ConfigValue<ConfTy>{key, std::forward<ConfTy>(conf), comment});
    return;
  }
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  conf_val = std::make_shared<ConfigValue<ConfTy>>(key, std::forward<ConfTy>(conf), std::move(comment));
  m_configs[key] = conf_val;
}
MYLIB_SPACE_END

#endif//MY_PROJECT_CONFIG_MANAGER_H

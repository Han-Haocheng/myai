#include "config_manager.h"

#include <functional>
#include <iostream>

MYLIB_SPACE_BEGIN

void ConfigManager::setConfigFile(const String &path) {
  m_config_file = path.empty() ? "config.yaml" : path;
  String file_type = m_config_file.substr(m_config_file.find_last_of("."));
  if (file_type == ".yaml" || file_type == ".yml") {
    m_config_file_type = CFT_YAML;
  } else if (file_type == ".json") {
    m_config_file_type = CFT_JSON;
  } else {
    m_config_file_type = CFT_UNKNOWN;
  }
}

void ConfigManager::showAllConfigs() {
  for (auto &conf_pair: m_configs) {
    std::cout << conf_pair.first << " : " << conf_pair.second->toString() << "\n";
  }
}

bool ConfigManager::load() {
  switch (m_config_file_type) {
    case CFT_UNKNOWN:
      return false;
    case CFT_YAML:
      load_from_yaml();
      break;
    case CFT_JSON:
      load_from_json();
      break;
  }
  return true;
}

bool ConfigManager::save() {
  switch (m_config_file_type) {
    case CFT_UNKNOWN:
      return false;
    case CFT_YAML:
      save_to_yaml();
      break;
    case CFT_JSON:
      save_to_json();
      break;
  }
  return true;
}

void ConfigManager::load_from_yaml() {
  try {
    YAML::Node root_node = YAML::LoadFile(m_config_file);

    std::function<void(const YAML::Node &, String)> recursive_func =
        [&recursive_func, this](const YAML::Node &node, String current_node_name) {
          if (!current_node_name.empty()) {
            current_node_name += ".";
          }

          std::cout << YAML::Dump(node) << std::endl;

          if (!node.IsMap()) {
            return;
          }

          for (auto &child_node: node) {
            String child_name = child_node.first.Scalar();
            if (child_name.empty()) {
              continue;
            }
            std::transform(child_name.begin(), child_name.end(), child_name.begin(), ::tolower);
            if (child_name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz_012345678") != std::string::npos) {
              //TODO: <log>
              continue;
            }
            String key = current_node_name + child_name;
            ConfigValueBase::ptr var = get_config_base(key);

            if (!var) {
              continue;
            }

            if (child_node.second.IsScalar()) {
              var->fromString(child_node.second.Scalar());
            } else {
              String val_str = YAML::Dump(child_node.second);
              var->fromString(val_str);
              recursive_func(child_node.second, key);
            }
          }
        };

    recursive_func(root_node, "");

  } catch (const YAML::BadFile &bf) {
    //TODO: <log>
    return;
  }
}
void ConfigManager::load_from_json() {
}
void ConfigManager::save_to_yaml() {
}
void ConfigManager::save_to_json() {
}

ConfigValueBase::ptr ConfigManager::get_config_base(const String &key) {
  auto fd_res = m_configs.find(key);
  if (fd_res == m_configs.end()) {
    return nullptr;
  }
  return fd_res->second;
}

MYLIB_SPACE_END

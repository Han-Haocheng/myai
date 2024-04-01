//
// Created by HanHaocheng on 2023/12/21.
//

#ifndef MY_LIB_CONFIG_H
#define MY_LIB_CONFIG_H

#include "../format/format.h"
#include <cstddef>
#include <iostream>
#include <memory>

namespace mylib {

//=====================================================================================================================

//=====================================================================================================================

class ConfigBase {
public:
  typedef std::shared_ptr<ConfigBase> ptr;
  ConfigBase(std::string name, std::string comment);
  virtual ~ConfigBase() = default;
  [[nodiscard]] const std::string &getName() const;
  [[nodiscard]] const std::string &getComment() const;

  [[nodiscard]] virtual std::string getTypeName() const = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  virtual void fromString(const std::string &str) = 0;

private:
  std::string m_name;
  std::string m_comment;
};

//=====================================================================================================================
//using ConfTy = ::myai::ThinkConfig;
//using Formatter = Formatter<ConfTy>;
template<typename ConfTy, typename Formatter = ::mylib::format::Formatter<ConfTy>>
class Config final : public ConfigBase {
public:
  typedef std::shared_ptr<Config<ConfTy>> ptr;
  using on_lisen_cb = std::function<void(const ConfTy &, const ConfTy &)>;
  Config(const std::string &name, const ConfTy &confVal, const std::string &comment)
      : ConfigBase(name, comment), m_val(new ConfTy{confVal}) {}

  [[nodiscard]] std::string getTypeName() const override { return typeid(*m_val).name(); }

  [[nodiscard]] std::string toString() const override {
    try {
      if (!m_val) {
        return "";
      }
      return Formatter{}.toString(*m_val);
    } catch (std::exception &e) {
      std::cout << "type cast failed, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast failed." << std::endl;
    }
    return "";
  }

  void fromString(const std::string &str) override {
    try {
      setValue(Formatter{}.fromString(str));
    } catch (std::exception &e) {
      std::cout << "type cast failed, "
                << "except=" << e.what() << std::endl;
    } catch (...) {
      std::cout << "type cast failed." << std::endl;
    }
  }

  void setValue(const ConfTy &val) {
    auto old = m_val;

    m_val.reset(new ConfTy{val});
    for (const auto &cb: m_cbs) {
      cb(*old, *m_val);
    }
  }
  //void setValue(ConfTy &&val) { m_val.reset(new ConfTy{std::move(val)}); }

  const ConfTy &getValue() { return *m_val; }

  size_t addListener(const on_lisen_cb &cb) {
    m_cbs.emplace_back(cb);
    return m_cbs.size() - 1;
  }

  bool delListener(size_t pos) {
#if __linux__
    return m_cbs.erase(m_cbs.begin() + pos) > 0;

#elif _WIN32
    return m_cbs.erase(m_cbs.begin() + pos) == m_cbs.end();
#endif// __linux__
  }

private:
  std::shared_ptr<ConfTy> m_val = {};
  std::vector<on_lisen_cb> m_cbs;
};
//=====================================================================================================================
class Configer {
public:
  typedef std::shared_ptr<Configer> ptr;
  using YamlPair = std::pair<std::string, YAML::Node>;
  typedef std::unordered_map<std::string, ConfigBase::ptr> ConfigMap;

  void loadByYaml(const std::string &path);
  //void saveToYaml(const std::string &path);

  template<typename ConfTy>
  typename Config<ConfTy>::ptr setConfig(const std::string &name, const ConfTy &val, const std::string &comment) {
    try {
      typename Config<ConfTy>::ptr fd_rt = getConfig<ConfTy>(name);
      if (!fd_rt) {
        fd_rt.reset(new Config<ConfTy>{name, val, comment});
        m_configs.emplace(name, fd_rt);
      }

      fd_rt->setValue(val);
      return fd_rt;
    } catch (std::exception &e) {
      std::cout << "<error Configer::setConfig>" << e.what() << std::endl;
    }
    return nullptr;
  }

  template<typename ConfTy>
  typename Config<ConfTy>::ptr getConfig(const std::string &name) {
    auto fd_rt = m_configs.find(name);
    if (fd_rt == m_configs.end()) {
      return nullptr;
    }
    if (fd_rt->second->getTypeName() != typeid(ConfTy).name()) {
      std::cout << "<error Configer::getConfig>get config failed,config type is difference"
                << "name=" << name << " "
                << "in type=" << typeid(ConfTy).name() << " "
                << "real type=" << fd_rt->second->getTypeName() << std::endl;
      return nullptr;
    }
    return std::dynamic_pointer_cast<Config<ConfTy>>(fd_rt->second);
  }

  bool delConfig(const std::string &name);

  static Configer::ptr GetInstance();

private:
  Configer();
  static void list_all_yaml_node(YAML::Node &root_node, std::list<YamlPair> &out);
  ConfigBase::ptr _getConfigBase(const std::string &name);

private:
  ConfigMap m_configs;
  std::vector<std::function<void(ConfigBase::ptr, ConfigBase::ptr &)>> m_cbs;
};
}// namespace mylib

#endif//MY_LIB_CONFIG_H

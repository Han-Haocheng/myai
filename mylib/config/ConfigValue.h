//
// Created by HanHaocheng on 2024/3/18.
//

#ifndef MY_AI_CONFIGVALUE_H
#define MY_AI_CONFIGVALUE_H

#include "../define/type.h"
#include "../format/format.h"
#include <memory>
MYLIB_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  ConfigValueBase(String name, String comment);
  virtual ~ConfigValueBase() = default;

  [[nodiscard]] const std::string &getName() const;
  [[nodiscard]] const std::string &getComment() const;

  [[nodiscard]] virtual std::string getTypeName() const = 0;
  [[nodiscard]] virtual std::string toString() const = 0;
  virtual void fromString(const std::string &str) = 0;

private:
  String m_name;
  String m_comment;
};

template<class ConTy>
class ConfigValue : public ConfigValueBase {
public:
  [[nodiscard]] std::string getTypeName() const override {
    return std::string();
  }
  [[nodiscard]] std::string toString() const override {
    return std::string();
  }
  void fromString(const std::string &str) override {
  }

private:
  ConTy m_config;
  Formatter<ConTy> m_formatter;
};

MYLIB_END
#endif//MY_AI_CONFIGVALUE_H

#ifndef MYLIB_CONFIG_VAL_H_
#define MYLIB_CONFIG_VAL_H_

#include "../mylib_def.h"

#include "../converter/ymal_converter.h"
#include <memory>
#include <string>
#include <utility>

MYLIB_SPACE_BEGIN

class ConfigValueBase {
public:
  using ptr = std::shared_ptr<ConfigValueBase>;

  ConfigValueBase(String name, String comment, String type_name)
      : m_type_name(std::move(type_name)), m_name(std::move(name)), m_comment(std::move(comment)) {
  }
  ~ConfigValueBase() = default;
  inline String name() const { return m_name; }
  inline String comment() const { return m_comment; }
  inline String typeName() const { return m_type_name; }

  virtual void fromString(String string) = 0;
  virtual String toString() const = 0;

protected:
  String m_type_name;
  String m_config_str;

private:
  String m_name;
  String m_comment;
};

template<typename ConfTy, typename CastFunc = YamlConverter<ConfTy>, typename = std::enable_if<is_converter<CastFunc>::value>>
class ConfigValue : public ConfigValueBase {
public:
  ConfigValue(String name, const ConfTy &conf, String comment)
      : ConfigValueBase(name, comment, typeid(ConfTy).name()), m_value() {
    m_value = conf;
    m_config_str = CastFunc{}.toString(m_value);
  }

  ~ConfigValue() = default;

  void fromString(String string) override {
    try {
      m_value = CastFunc{}.fromString(string);
    } catch (const std::exception &e) {
      // TODO: <log>error
    }
  }

  String toString() const override {
    try {
      return CastFunc{}.toString(m_value);
    } catch (const std::exception &e) {
      // TODO: <log>error
    }
    return "";
  }

  ConfTy config() const { return m_value; }

private:
  ConfTy m_value;
};

MYLIB_SPACE_END

#endif

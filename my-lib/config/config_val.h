#pragma once
#include "../mylib_def.h"

#include <memory>
#include <string>

MYLIB_SPACE_BEGIN


class ConfigValueBase {
public:
  ConfigValueBase(String name, String comment);
  ~ConfigValueBase();
  inline String name() const { return m_name; }
  inline String comment() const { return m_comment; }
  inline String typeName() const { return m_type_name; }


  virtual void fromString(String string) = 0;
  virtual String toString() const        = 0;

protected:
  String m_type_name;

private:
  String m_name;
  String m_comment;
};


ConfigValueBase::ConfigValueBase(String name, String comment)
    : m_name(name), m_comment(comment), m_type_name("null") {
}


ConfigValueBase::~ConfigValueBase() {
}


template<typename ConfTy, typename JsonCastFunc>
class ConfigValue : public ConfigValueBase {
public:
  ConfigValue(String name, const ConfTy &conf, String comment);
  ConfigValue(String name, String confStr, String comment);
  ~ConfigValue();

  void fromString(String string) override;
  String toString() const override;

private:
  std::shared_ptr<ConfTy> m_value;
};

template<typename ConfTy, typename JsonCastFunc>
inline ConfigValue<ConfTy, JsonCastFunc>::ConfigValue(String name, const ConfTy &conf, String comment)
    : ConfigValueBase(name, comment), m_type_name(typeid(String).name()), m_value(nullptr) {
  m_value.reset(new ConfTy(conf));
}
template<typename ConfTy, typename JsonCastFunc>
inline ConfigValue<ConfTy, JsonCastFunc>::ConfigValue(String name, String confStr, String comment)
    : ConfigValueBase(name, comment), m_type_name(typeid(String).name()), m_value(nullptr) {
  m_value.reset(new ConfTy{JsonCastFunc{}.fromString(confStr)});
}

template<typename ConfTy, typename JsonCastFunc>
inline ConfigValue<ConfTy, JsonCastFunc>::~ConfigValue() {
}

template<typename ConfTy, typename JsonCastFunc>
inline void ConfigValue<ConfTy, JsonCastFunc>::fromString(String string) {
  try {
    (*m_value) = JsonCastFunc{}.formString(string)
  } catch (const std::exception &e) {
    // TODO: <log>error
  }
}

template<typename ConfTy, typename JsonCastFunc>
inline String ConfigValue<ConfTy, JsonCastFunc>::toString() const {
  try {
    return JsonCastFunc{}.toString(*m_value)
  } catch (const std::exception &e) {
    // TODO: <log>error
  }
  return "";
}

MYLIB_SPACE_END
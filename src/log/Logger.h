//
// Created by HanHaocheng on 2023/11/5.
//

#ifndef MY_AI_LOGGER_H
#define MY_AI_LOGGER_H

#include "../config/Configer.h"
#include "../time/Timer.h"

#include "LogAppender.h"
#include "LogEvent.h"
#include "LogFormatter.h"
#include <memory>
#include <string>
#include <utility>

namespace myai
{
struct AppenderConfigVal {
  LogAppender::Type type;
  std::string file;
  LogLevel level;
  std::string pattern;
};

struct LoggerConfigVal {
  std::string name;
  LogLevel level;
  std::string pattern;
  std::vector<AppenderConfigVal> appenders;
};

class Logger
{
public:
  typedef std::shared_ptr<Logger> ptr;
  Logger(std::string name, LogLevel logger_level);
  ~Logger();

  void log(const LogEvent::ptr &event);

  void addAppender(const LogAppender::ptr &appender);
  void delAppender(const LogAppender::ptr &appender);
  void clearAppenders();

  const std::string &getName() const { return m_name; }
  LogLevel getLevel() const { return m_level; }
  void setName(const std::string &name) { m_name = name; }
  void setLevel(LogLevel level) { m_level = level; }

  LogFormatter::ptr getFormatter() const { return m_formatter; }
  void setFormatter(LogFormatter::ptr formatter) { m_formatter = std::move(formatter); }

private:
  std::string m_name;
  LogLevel m_level = LogLevel::DEBUG;
  LogFormatter::ptr m_formatter = nullptr;
  std::vector<LogAppender::ptr> m_appenders;
};

//========================================================================================

template<>
class ConfigCast<AppenderConfigVal, std::string>
{
public:
  std::string operator()(const AppenderConfigVal &val) const
  {
    YAML::Node node;
    if (val.type == LogAppender::APD_UNDEFINE) {
      throw std::runtime_error("Appender type is undefined.");
    }

    node["type"] = LogAppender::ToString(val.type);

    if (val.type == LogAppender::APD_FILE) {
      node["file"] = val.file.empty() ? "./log.txt" : val.file;
    }
    if (val.level != LogLevel::UNKNOWN) {
      node["level"] = toString(val.level);
    }
    if (!val.pattern.empty()) {
      node["pattern"] = val.pattern;
    }
    return YAML::Dump(node);
  }
};

template<>
class ConfigCast<std::string, AppenderConfigVal>
{
public:
  AppenderConfigVal operator()(const std::string &val) const
  {
    YAML::Node node = YAML::Load(val);
    AppenderConfigVal config_val = {
        LogAppender::FromString(node["type"].as<std::string>()),
        node["file"].as<std::string>(),
        fromString(node["level"].as<std::string>()),
        node["pattern"].as<std::string>(),
    };
    return config_val;
  }
};

template<>
class ConfigCast<LoggerConfigVal, std::string>
{
public:
  std::string operator()(const LoggerConfigVal &val) const
  {

    YAML::Node node;
    if (val.name.empty()) {
      throw std::runtime_error("Logger name is undefined.");
    }
    node["name"] = val.name;

    if (val.level != LogLevel::UNKNOWN) {
      node["level"] = toString(val.level);
    }
    if (!val.pattern.empty()) {
      node["pattern"] = val.pattern;
    }
    if (val.appenders.empty()) {
      node.push_back(YAML::Load(ConfigCast<std::vector<AppenderConfigVal>, std::string>()(val.appenders)));
    }
    return YAML::Dump(node);
  }
};

template<>
class ConfigCast<std::string, LoggerConfigVal>
{
public:
  LoggerConfigVal operator()(const std::string &val) const
  {
    YAML::Node node = YAML::Load(val);
    LoggerConfigVal logger_config = {
        node["name"].as<std::string>(),
        fromString(node["level"].as<std::string>()),
        node["pattern"].as<std::string>(),
        ConfigCast<std::string, std::vector<AppenderConfigVal>>{}(node["appenders"].as<std::string>()),
    };

    return logger_config;
  }
};
}// namespace myai

#endif//MY_AI_LOGGER_H

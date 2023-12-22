//
// Created by HanHaocheng on 2023/11/6.
//

#ifndef MY_AI_LOGGERMANAGER_H
#define MY_AI_LOGGERMANAGER_H

#include "../asyn//Coroutine.h"
#include "../asyn/Thread.h"
#include "../config/Configer.h"
#include "Logger.h"
#include <memory>
#include <unordered_map>

#define MYAI_LOG_BASE(logger, level)                                                                                                                                                         \
  if (logger->getLevel() <= level)                                                                                                                                                           \
  myai::LogWarp{                                                                                                                                                                             \
      logger, std::make_shared<myai::LogEvent>(myai::Timer::GetCurrentTime(), __FILE__, __LINE__, logger->getName(), level, "nonameThread", myai::Thread::GetId(), myai::Coroutine::GetId(), \
                                               std::string{__PRETTY_FUNCTION__})}                                                                                                            \
      .msg()

#define MYAI_LOG_DEBUG(logger) MYAI_LOG_BASE(logger, myai::LogLevel::LL_DEBUG)
#define MYAI_LOG_INFO(logger) MYAI_LOG_BASE(logger, myai::LogLevel::LL_INFO)
#define MYAI_LOG_WARN(logger) MYAI_LOG_BASE(logger, myai::LogLevel::LL_WARN)
#define MYAI_LOG_ERROR(logger) MYAI_LOG_BASE(logger, myai::LogLevel::LL_ERROR)

#define MYAI_LOGGER_ROOT myai::LoggerManager::GetInstance()->getRootLogger()
#define MYAI_LOGGER_NAME(name) myai::LoggerManager::GetInstance()->addLogger(name, myai::LogLevel::LL_DEBUG)

namespace myai
{

///=====================================================================================================================
/// AppenderConfigVal
///=====================================================================================================================

struct AppenderConfigVal {
  LogAppender::Type type;
  std::string file;
  LogLevel level;
  std::string pattern;
};
template<>
class ConfigCast<AppenderConfigVal, std::string>
{
public:
  std::string operator()(const AppenderConfigVal &val) const
  {
    YAML::Node node;
    if (val.type == LogAppender::APD_UNDEFINE) { throw std::runtime_error("Appender type is undefined."); }
    node["type"] = LogAppender::ToString(val.type);
    if (val.type == LogAppender::APD_FILE) { node["file"] = val.file.empty() ? "./log.txt" : val.file; }
    if (val.level != LogLevel::LL_UNKNOWN) { node["level"] = toString(val.level); }
    if (!val.pattern.empty()) { node["pattern"] = val.pattern; }
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
    AppenderConfigVal ret{};
    if (!node["type"].IsDefined() || LogAppender::APD_UNDEFINE == (ret.type = LogAppender::FromString(node["type"].as<std::string>()))) {
      std::cout << "<error ConfigCast<std::string, AppenderConfigVal>>appender type define bad." << std::endl;
    }
    if (ret.type == LogAppender::APD_FILE) {
      ret.file = node["file"].IsDefined() ? node["file"].as<std::string>() : "";
    }

    ret.level = node["level"].IsDefined() ? fromString(node["level"].as<std::string>()) : LogLevel::LL_UNKNOWN;
    ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
    return ret;
  }
};

///=====================================================================================================================
/// LoggerConfigVal
///=====================================================================================================================

struct LoggerConfigVal {
  std::string name;
  LogLevel level;
  std::string pattern;
  std::vector<AppenderConfigVal> appenders;
  bool operator<(const LoggerConfigVal &rhs) const { return name < rhs.name; }
  bool operator>(const LoggerConfigVal &rhs) const { return rhs < *this; }
  bool operator<=(const LoggerConfigVal &rhs) const { return !(rhs < *this); }
  bool operator>=(const LoggerConfigVal &rhs) const { return !(*this < rhs); }
  bool operator==(const LoggerConfigVal &rhs) const { return name == rhs.name; }
  bool operator!=(const LoggerConfigVal &rhs) const { return !(rhs == *this); }
};

template<>
class ConfigCast<LoggerConfigVal, std::string>
{
public:
  std::string operator()(const LoggerConfigVal &val) const
  {
    YAML::Node node;
    if (val.name.empty()) { throw std::runtime_error("Logger name is undefined."); }
    node["name"] = val.name;
    if (val.level != LogLevel::LL_UNKNOWN) { node["level"] = toString(val.level); }
    if (!val.pattern.empty()) { node["pattern"] = val.pattern; }
    if (val.appenders.empty()) { node.push_back(YAML::Load(ConfigCast<std::vector<AppenderConfigVal>, std::string>()(val.appenders))); }
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
    if (!node["name"].IsDefined()) {
      std::cout << "<error ConfigCast>LoggerConfigVal string undefine name";
    }
    LoggerConfigVal ret{};

    ret.name = node["name"].as<std::string>();
    ret.level = node["level"].IsDefined() ? fromString(node["level"].as<std::string>()) : LogLevel::LL_UNKNOWN;
    ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
    if (node["appenders"].IsDefined()) {
      ret.appenders = ConfigCast<std::string, std::vector<AppenderConfigVal>>{}(Dump(node["appenders"]));
    }

    return ret;
  }
};

///=====================================================================================================================
/// LogConfigVal
///=====================================================================================================================

struct LogConfigVal {
  LogLevel def_level;
  std::string def_pattern;

  LoggerConfigVal root_logger;
  std::set<LoggerConfigVal> loggers;
};

template<>
class ConfigCast<LogConfigVal, std::string>
{
public:
  std::string operator()(const LogConfigVal &val) const
  {
    YAML::Node node;
    node["def_level"] = toString(val.def_level);

    if (!val.def_pattern.empty()) {
      node["def_pattern"] = val.def_pattern;
    }
    node["root_logger"] = ConfigCast<LoggerConfigVal, std::string>{}(val.root_logger);
    if (!val.loggers.empty()) {
      node["loggers"] = ConfigCast<std::set<LoggerConfigVal>, std::string>{}(val.loggers);
    }
    return YAML::Dump(node);
  }
};
template<>
class ConfigCast<std::string, LogConfigVal>
{
public:
  LogConfigVal operator()(const std::string &val) const
  {
    YAML::Node node = YAML::Load(val);
    LogConfigVal ret{};

    ret.def_level = node["def_level"].IsDefined() ? fromString(node["def_level"].as<std::string>()) : LogLevel::LL_UNKNOWN;
    ret.def_pattern = node["def_pattern"].IsDefined() ? node["def_pattern"].as<std::string>() : "";
    if (node["root_logger"].IsDefined()) {
      ret.root_logger = ConfigCast<std::string, LoggerConfigVal>{}(Dump(node["root_logger"]));
    }
    if (node["loggers"].IsDefined()) {
      ret.loggers = ConfigCast<std::string, std::set<LoggerConfigVal>>{}(Dump(node["loggers"]));
    }
    return ret;
  }
};

///=====================================================================================================================

class LogWarp
{
public:
  LogWarp(Logger::ptr logger, LogEvent::ptr event);
  ~LogWarp();
  std::stringstream &msg() { return m_event->msg(); }

private:
  Logger::ptr m_logger = nullptr;
  LogEvent::ptr m_event = nullptr;
};

///=====================================================================================================================

class LoggerManager
{
public:
  typedef std::shared_ptr<LoggerManager> ptr;

  Logger::ptr setLogger(const std::string &name,
                        LogLevel logger_level,
                        LogFormatter::ptr formatter = nullptr,
                        const std::vector<LogAppender::ptr> &appenders = {});
  bool setLogger(const Logger::ptr &logger);
  Logger::ptr getLogger(const std::string &name);
  Logger::ptr addLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter = nullptr, const std::vector<LogAppender::ptr> &appenders = {});
  bool addLogger(const Logger::ptr &logger);
  bool delLogger(const std::string &name);

  const Logger::ptr &getRootLogger() const { return m_rootLogger; }

  static LoggerManager::ptr GetInstance();

  bool isInitialized() const { return m_isInitialized; }

private:
  explicit LoggerManager();
  void init();
  Logger::ptr config_logger(const LoggerConfigVal &conf);
  static LogAppender::ptr config_appender(LogLevel logger_level, LogFormatter::ptr logger_formatter, const AppenderConfigVal &conf);

private:
  bool m_isInitialized = false;

  Logger::ptr m_rootLogger = nullptr;
  std::vector<LoggerConfigVal> m_loggerConfigs{};
  std::unordered_map<std::string, Logger::ptr> m_loggers{};

  LogLevel m_defLoggerLevel = LogLevel::LL_UNKNOWN;
  LogFormatter::ptr m_defLoggerFormater = nullptr;
};

}// namespace myai

#endif//MY_AI_LOGGERMANAGER_H

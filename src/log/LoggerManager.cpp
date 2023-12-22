//
// Created by HanHaocheng on 2023/11/6.
//

#include "LoggerManager.h"
#include <iostream>
#include <memory>
#include <utility>

namespace myai
{

Config<LogConfigVal>::ptr g_log_configs =
    Configer::GetInstance()->setConfig<LogConfigVal>("log",
                                                     {
                                                         LogLevel::LL_DEBUG,
                                                         "%d{%Y-%m-%d %H:%M:%S} %m%n",
                                                         {
                                                             "root",
                                                             LogLevel::LL_DEBUG,
                                                             "%d{%Y-%m-%d %H:%M:%S} %t%T%F%T%N [%p] [%c] %f:%l %M ==> %m%n",
                                                             {
                                                                 {
                                                                     LogAppender::Type::APD_CONSOLE,
                                                                     "",
                                                                     LogLevel::LL_DEBUG,
                                                                     "",
                                                                 },
                                                             },
                                                         },
                                                         {},
                                                     },
                                                     "log");
LoggerManager::ptr LoggerManager::GetInstance()
{
  static LoggerManager::ptr manager{new LoggerManager{}};
  if (manager == nullptr) {
    manager.reset(new LoggerManager{});
  }
  if (!manager->isInitialized()) {
    manager->init();
  }

  return manager;
}

LoggerManager::LoggerManager() = default;

void LoggerManager::init()
{
  auto conf = g_log_configs->getValue();
  m_defLoggerLevel = conf.def_level;
  m_defLoggerFormater = conf.def_pattern.empty() ? nullptr : std::make_shared<LogFormatter>(conf.def_pattern);
  m_rootLogger = config_logger(g_log_configs->getValue().root_logger);

  this->addLogger(m_rootLogger);

  g_log_configs->addListener([this](const LogConfigVal &oldConfs, const LogConfigVal &newConfs) {
    m_defLoggerLevel = newConfs.def_level;
    m_defLoggerFormater->setPattern(newConfs.def_pattern);

    for (const auto &new_logger_conf: newConfs.loggers) {
      auto fd_res = oldConfs.loggers.find(new_logger_conf);
      if (fd_res != oldConfs.loggers.end()) {
        this->setLogger(config_logger(new_logger_conf));
      }
      this->addLogger(config_logger(new_logger_conf));
    }
    for (const auto &old_conf: oldConfs.loggers) {
      auto fd_res = newConfs.loggers.find(old_conf);
      if (fd_res == newConfs.loggers.end()) {
        this->delLogger(fd_res->name);
      }
    }
  });
  Configer::GetInstance()->loadByYaml("log.yml");
  m_isInitialized = true;
}

Logger::ptr LoggerManager::getLogger(const std::string &name)
{
  auto find_rt = m_loggers.find(name);
  if (find_rt == m_loggers.end()) {
    return nullptr;
  }
  return find_rt->second;
}

Logger::ptr LoggerManager::setLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter, const std::vector<LogAppender::ptr> &appenders)
{
  auto logger = getLogger(name);
  if (!logger)
    return logger;
  if (logger_level != LogLevel::LL_UNKNOWN)
    logger->setLevel(logger_level);
  if (formatter)
    logger->setFormatter(std::move(formatter));
  for (const auto &appender: appenders)
    logger->addAppender(appender);
  return logger;
}

bool LoggerManager::setLogger(const Logger::ptr &logger)
{
  if (logger == nullptr || logger->getName().empty()) {
    return false;
  }
  auto fd_res = m_loggers.find(logger->getName());
  if (fd_res == m_loggers.end()) {
    return false;
  }
  fd_res->second = logger;
  return false;
}

Logger::ptr LoggerManager::addLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter, const std::vector<LogAppender::ptr> &appenders)
{
  auto res = getLogger(name);
  if (res) {
    return res;
  }

  Logger::ptr logger{new Logger(name, LogLevel::LL_DEBUG)};
  if (formatter)
    logger->setFormatter(std::move(formatter));

  for (const auto &appender: appenders)
    logger->addAppender(appender);

  return m_loggers.emplace(logger->getName(), logger).first->second;
}
bool LoggerManager::addLogger(const Logger::ptr &logger)
{
  auto res = getLogger(logger->getName());
  if (res) {
    return false;
  }
  m_loggers.emplace(logger->getName(), logger);
  return true;
}

bool LoggerManager::delLogger(const std::string &name)
{
  auto findRt = m_loggers.find(name);
  if (findRt != m_loggers.end()) {
    return false;
  }
  m_loggers.erase(findRt);
  return true;
}

Logger::ptr LoggerManager::config_logger(const LoggerConfigVal &conf)
{
  if (conf.name.empty()) {
    return nullptr;
  }
  auto level = conf.level == LogLevel::LL_UNKNOWN ? m_defLoggerLevel : conf.level;

  Logger::ptr ret = std::make_shared<Logger>(conf.name, level);
  auto formatter = conf.pattern.empty() ? m_defLoggerFormater : std::make_shared<LogFormatter>(conf.pattern);
  ret->setFormatter(formatter);
  for (const auto &app_conf: conf.appenders) {
    auto app = config_appender(level, formatter, app_conf);
    if (app == nullptr) { continue; }
    ret->addAppender(app);
  }
  return ret;
}

LogAppender::ptr LoggerManager::config_appender(LogLevel logger_level, LogFormatter::ptr logger_formatter, const AppenderConfigVal &conf)
{
  if (conf.type == LogAppender::APD_UNDEFINE) {
    return nullptr;
  }
  auto formatter = conf.pattern.empty() ? std::move(logger_formatter) : std::make_shared<LogFormatter>(conf.pattern);
  LogLevel level = conf.level == LogLevel::LL_UNKNOWN ? logger_level : conf.level;
  LogAppender::ptr ret = nullptr;
  if (conf.type == LogAppender::APD_CONSOLE) {
    ret = std::make_shared<ConsoleAppender>(level);
    ret->setFormatter(formatter);
  } else if (conf.type == LogAppender::APD_FILE) {
    ret = std::make_shared<FileAppender>(level, conf.file);
    ret->setFormatter(formatter);
  }
  return ret;
}

}// namespace myai
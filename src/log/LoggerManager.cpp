//
// Created by HanHaocheng on 2023/11/6.
//

#include "LoggerManager.h"
#include <iostream>
#include <memory>
#include <utility>

namespace myai
{

LoggerManager::ptr LoggerManager::GetInstance()
{
  static LoggerManager::ptr manager{new LoggerManager()};
  if (manager == nullptr) {
    manager.reset(new LoggerManager{});
  }
  return manager;
}

LoggerManager::LoggerManager()
    : m_configer(new Configer),
      m_defFormatter(new LogFormatter{"%d{%Y-%m-%d %H:%MutexType:%S} %t%T%F%T%N [%p] [%c] %f:%l %M->%m%n"}),
      m_rootLogger(new Logger("root", LogLevel::DEBUG))
{
  m_rootLogger->setFormatter(m_defFormatter);
  m_rootLogger->addAppender(std::make_shared<ConsoleAppender>(LogLevel::DEBUG));
  addLogger(m_rootLogger);

  //loadByYaml(std::string());
}

void LoggerManager::loadByYaml(const std::string &path)
{
  m_configer->loadByYaml(path);
  auto getRt = m_configer->getConfig<std::vector<LoggerConfigVal>>("loggers");
  if (getRt != nullptr) {
    m_loggerConfigs = getRt->getValue();
  }

  for (const auto &log_conf: m_loggerConfigs) {
    std::vector<LogAppender::ptr> tmpAppenders;
    for (const auto &app_conf: log_conf.appenders) {
      switch (app_conf.type) {
        case LogAppender::APD_UNDEFINE:
          throw std::runtime_error{"appender type undefine"};
          break;
        case LogAppender::APD_CONSOLE:
          tmpAppenders.emplace_back(std::make_shared<ConsoleAppender>(app_conf.level));
          if (!app_conf.pattern.empty()) {
            tmpAppenders.back()->setFormatter(std::make_shared<LogFormatter>(app_conf.pattern));
          }
          break;
        case LogAppender::APD_FILE:
          tmpAppenders.emplace_back(std::make_shared<FileAppender>(app_conf.level, app_conf.file));
          if (!app_conf.pattern.empty()) {
            tmpAppenders.back()->setFormatter(std::make_shared<LogFormatter>(app_conf.pattern));
          }
          break;
      }
    }
    addLogger(log_conf.name, log_conf.level, std::make_shared<LogFormatter>(log_conf.pattern), tmpAppenders);
  }
}

Logger::ptr LoggerManager::getLogger(const std::string &name)
{
  auto find_rt = m_loggers.find(name);
  if (find_rt == m_loggers.end()) {
    return nullptr;
  }
  return find_rt->second;
}

bool LoggerManager::setLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter, const std::vector<LogAppender::ptr> &appenders)
{
  auto logger = getLogger(name);
  if (!logger)
    return false;
  if (logger_level != LogLevel::UNKNOWN)
    logger->setLevel(logger_level);
  if (formatter)
    logger->setFormatter(std::move(formatter));
  for (const auto &appender: appenders)
    logger->addAppender(appender);
  return true;
}

Logger::ptr LoggerManager::addLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter, const std::vector<LogAppender::ptr> &appenders)
{
  auto res = getLogger(name);
  if (res) {
    return res;
  }

  Logger::ptr logger{new Logger(name, LogLevel::DEBUG)};
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
  if (!logger->getFormatter()) {
    logger->setFormatter(m_defFormatter);
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

}// namespace myai
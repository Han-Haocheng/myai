//
// Created by HanHaocheng on 2023/11/6.
//

#ifndef MY_AI_LOGGERMANAGER_H
#define MY_AI_LOGGERMANAGER_H

#include "../config/Configer.h"
#include "Logger.h"
#include <memory>
#include <unordered_map>

#define MYAI_LOG_BASE(logger, level)                                                                                                                         \
  if (logger->getLevel() <= level)                                                                                                                           \
  myai::LogWarp{                                                                                                                                             \
      logger, std::make_shared<myai::LogEvent>(myai::Timer::GetCurrentTime(), __FILE__, __LINE__, logger->getName(), level, "nonameThread", 0, 0, \
                                               std::string{__PRETTY_FUNCTION__}.substr(std::string(__PRETTY_FUNCTION__).find(' ')))}                         \
      .msg()

#define MYAI_LOG_DEBUG(logger) MYAI_LOG_BASE(logger, myai::LogLevel::DEBUG)
#define MYAI_LOG_INFO(logger) MYAI_LOG_BASE(logger, myai::LogLevel::INFO)
#define MYAI_LOG_WARN(logger) MYAI_LOG_BASE(logger, myai::LogLevel::WARN)
#define MYAI_LOG_ERROR(logger) MYAI_LOG_BASE(logger, myai::LogLevel::ERROR)

#define MYAI_LOGGER_ROOT myai::LoggerManager::GetInstance()->getRootLogger()
#define MYAI_LOGGER_NAME(name) myai::LoggerManager::GetInstance()->addLogger(name, myai::LogLevel::DEBUG)

namespace myai
{

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

class LoggerManager
{
public:
  typedef std::shared_ptr<LoggerManager> ptr;

  bool setLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter = nullptr, const std::vector<LogAppender::ptr> &appenders = {});
  Logger::ptr getLogger(const std::string &name);
  Logger::ptr addLogger(const std::string &name, LogLevel logger_level, LogFormatter::ptr formatter = nullptr, const std::vector<LogAppender::ptr> &appenders = {});
  bool addLogger(const Logger::ptr& logger);
  bool delLogger(const std::string &name);

  const Logger::ptr &getRootLogger() const { return m_rootLogger; }

  static LoggerManager::ptr GetInstance();

  void loadByYaml(const std::string &path);

private:
  explicit LoggerManager();

private:
  Configer::ptr m_configer = nullptr;
  LogFormatter::ptr m_defFormatter = nullptr;
  Logger::ptr m_rootLogger = nullptr;
  std::vector<LoggerConfigVal> m_loggerConfigs{};
  std::unordered_map<std::string, Logger::ptr> m_loggers{};
};

}// namespace myai

#endif//MY_AI_LOGGERMANAGER_H

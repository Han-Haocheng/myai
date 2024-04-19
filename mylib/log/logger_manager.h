//
// Created by HanHaocheng on 2024/4/12.
//

#ifndef MYPROJECT_LOGGER_MANAGER_H
#define MYPROJECT_LOGGER_MANAGER_H
#include <memory>

#include "../untils/until.h"
#include "logger.h"

#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->try_getLogger(name)

#define MYLIB_LOG_BASE(level, logger)                                           \
  if (logger)                                                                   \
  mylib::_LogWarp{                                                              \
      logger,                                                                   \
      std::make_shared<mylib::LogEvent>(logger->getName(),                      \
                                        mylib::LogEvent::LE_##level,            \
                                        __LINE__,                               \
                                        __FILE__,                               \
                                        __func__,                               \
                                        __CLASS_NAME__, \
                                        mylib::GetCurrentTimestamp(),           \
                                        1, 1,                                   \
                                        "String threadName",                    \
                                        "String coroutineName")}                \
      ._msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(DEBUG, logger)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(INFO, logger)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(WARN, logger)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(ERROR, logger)

MYLIB_SPACE_BEGIN

class _LogWarp {

public:
  inline _LogWarp(Logger::ptr l, LogEvent::ptr e) : m_logger(l), m_event(e) {}
  inline ~_LogWarp() { m_logger->log(m_event); }

  inline SString &_msg() const { return m_event->msg(); }

private:
  Logger::ptr m_logger;
  LogEvent::ptr m_event;
};

class LoggerManager {
public:
  using ptr = std::shared_ptr<LoggerManager>;
  static ptr GetInstance();

  void addLogger(const Logger::ptr &logger);
  Logger::ptr addLogger(const String &name, LogEvent::Type level, LogFormatter::ptr formatter);
  [[nodiscard]] Logger::ptr getLogger(const String &name);
  [[nodiscard]] Logger::ptr try_getLogger(const String &name);

private:
  void _add_logger(const Logger::ptr &logger);
  Logger::ptr _get_logger(const String &name);

private:
  LogFormatter::ptr m_def_formatter;

  std::unordered_map<String, Logger::ptr> m_loggers;

  static LoggerManager::ptr s_instance;
};
MYLIB_SPACE_END
#endif//MYPROJECT_LOGGER_MANAGER_H

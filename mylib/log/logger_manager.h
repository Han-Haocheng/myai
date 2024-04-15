//
// Created by HanHaocheng on 2024/4/12.
//

#ifndef MYPROJECT_LOGGER_MANAGER_H
#define MYPROJECT_LOGGER_MANAGER_H
#include <memory>

#include "logger.h"
#define MYLIB_LOG_DEF_ROOT_NAME "root"
#define MYLIB_LOG_DEF_FORMATTER_PATTERN "[%d{%Y-%m-%d %H-%M-%S}][%f:%l]%T%p%T%c%T%t%T%m%n"
#define MYLIB_LOG_DEF_LEVEL LogEvent::LE_DEBUG
#define MYLIB_LOG_NAME(name) mylib::LoggerManager::GetInstance()->addLogger(name)
#define MYLIB_LOG_ROOT mylib::LoggerManager::GetInstance()->get_root();

MYLIB_SPACE_BEGIN

class LoggerManager {
public:
  using ptr = std::shared_ptr<LoggerManager>;
  static ptr GetInstance();

  inline Logger::ptr get_root() { return m_root_logger; }

  void addLogger(const Logger::ptr &logger);
  Logger::ptr addLogger(const String &name, LogEvent::Type level = MYLIB_LOG_DEF_LEVEL, const LogFormatter::ptr &formatter = std::make_shared<LogFormatter>(MYLIB_LOG_DEF_FORMATTER_PATTERN));
  [[nodiscard]] Logger::ptr getLogger(const String &name);

private:
  void _add_logger(const Logger::ptr &logger);
  Logger::ptr _get_logger(const String &name);

private:
  LogFormatter::ptr m_def_formatter;
  Logger::ptr m_root_logger;

  std::unordered_map<String, Logger::ptr> m_loggers;

  static LoggerManager::ptr s_instance;
};
MYLIB_SPACE_END
#endif//MYPROJECT_LOGGER_MANAGER_H

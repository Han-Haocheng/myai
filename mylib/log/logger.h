//
// Created by HanHaocheng on 2024/4/10.
//

#ifndef MYPROJECT_LOGGER_H
#define MYPROJECT_LOGGER_H
#include <iostream>
#include <utility>

#include "log_appender.h"
#include "log_event.h"
#include "log_formatter.h"

MYLIB_SPACE_BEGIN

#define MYLIB_LOGGER_ROOT_NAME "root"
#define MYLIB_DEF_LOGGER_LEVEL MYLIB_SPACE_NAME::LogEvent::LE_DEBUG
#define MYLIB_ROOT_LOGGER MYLIB_SPACE_NAME::Logger::RootLogger()

class Logger {
public:
  using ptr = std::shared_ptr<Logger>;

  Logger(String name,
         LogEvent::Type level,
         LogFormatter::ptr formatter)
      : m_name(std::move(name)), m_type(level), m_appenders(), m_formatter(std::move(formatter)) {}

  [[nodiscard]] inline const String &getName() const { return m_name; }
  inline void setName(const String &name) { m_name = name; }
  [[nodiscard]] inline const LogFormatter::ptr &getFormatter() const { return m_formatter; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }

  void addAppender(LogAppender::ptr appender);

  void log(const LogEvent::ptr &event);

  static Logger::ptr RootLogger() {
    if (!m_root_logger) {
      m_root_logger.reset(new Logger{
          MYLIB_LOGGER_ROOT_NAME,
          MYLIB_DEF_LOGGER_LEVEL,
          MYLIB_DEF_LOG_FORMATTER});
      m_root_logger->addAppender(ConsoleAppender::ptr{new ConsoleAppender{MYLIB_DEF_LOGGER_LEVEL, MYLIB_DEF_LOG_FORMATTER}});
    }
    return m_root_logger;
  }

private:
  String m_name;

  LogEvent::Type m_type;
  std::vector<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
  static Logger::ptr m_root_logger;
};
MYLIB_SPACE_END
#endif//MYPROJECT_LOGGER_H

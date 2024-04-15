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
class Logger {
public:
  using ptr = std::shared_ptr<Logger>;

  Logger(String name, LogEvent::Type level, LogFormatter::ptr formatter)
      : m_name(std::move(name)), m_type(level), m_appenders(), m_formatter(std::move(formatter)) {}

  [[nodiscard]] const String &getName() const { return m_name; }
  void setName(const String &name) { m_name = name; }
  [[nodiscard]] const LogFormatter::ptr &getFormatter() const { return m_formatter; }
  void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }

  void addAppender(LogAppender::ptr appender) {
    if (!appender) {
      return;
    }

    appender->setFormatter(m_formatter);
    m_appenders.emplace_back(appender);
  }

  void log(const LogEvent::ptr &event) {
    if (event->getType() < m_type) {
      return;
    }

    if (m_appenders.empty()) {
      std::cout << "appender is empty" << std::endl;
      return;
    }

    for (auto &appender: m_appenders) {
      appender->append(event);
    }
  }

private:
  String m_name;

  LogEvent::Type m_type;
  std::vector<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
};
MYLIB_SPACE_END
#endif//MYPROJECT_LOGGER_H

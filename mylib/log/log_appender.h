//
// Created by HanHaocheng on 2024/4/10.
//

#ifndef MYPROJECT_LOG_APPENDER_H
#define MYPROJECT_LOG_APPENDER_H
#include <fstream>
#include <utility>

#include "log_event.h"
#include "log_formatter.h"

MYLIB_SPACE_BEGIN

class LogAppender {
public:
  LogAppender() : m_type(LogEvent::Type::LE_UNDEFINED), m_formatter(nullptr) {}
  explicit LogAppender(LogEvent::Type type, LogFormatter::ptr formatter = nullptr) : m_type(type), m_formatter(std::move(formatter)) {}

  using ptr = std::shared_ptr<LogAppender>;

  virtual void append(const LogEvent::ptr &event) = 0;

  inline void setType(LogEvent::Type type) { m_type = type; }
  inline void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
  [[nodiscard]] inline LogEvent::Type getType() const { return m_type; }
  [[nodiscard]] inline LogFormatter::ptr getFormatter() const { return m_formatter; }

protected:
  LogEvent::Type m_type;
  LogFormatter::ptr m_formatter;
};

class ConsoleAppender : public LogAppender {
public:
  explicit ConsoleAppender(LogEvent::Type type, LogFormatter::ptr formatter = nullptr) : LogAppender(type, std::move(formatter)) {}
  void append(const LogEvent::ptr &event) override;
};

class FileAppender : public LogAppender {
public:
  FileAppender(String file, LogEvent::Type type, LogFormatter::ptr formatter = nullptr) : LogAppender(type, std::move(formatter)), m_file_path(std::move(file)) {}
  void append(const LogEvent::ptr &event) override;


private:
  std::ofstream fs;
  String m_file_path;
};

MYLIB_SPACE_END
#endif//MYPROJECT_LOG_APPENDER_H

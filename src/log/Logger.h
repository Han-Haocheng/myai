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
  LogLevel m_level = LogLevel::UNKNOWN;
  LogFormatter::ptr m_formatter = nullptr;
  std::vector<LogAppender::ptr> m_appenders;
};

//========================================================================================


}// namespace myai

#endif//MY_AI_LOGGER_H

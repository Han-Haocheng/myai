//
// Created by HanHaocheng on 2023/11/4.
//

#include "LogEvent.h"

#include <algorithm>
#include <utility>

namespace myai
{
std::string toString(LogLevel level)
{
  switch (level) {
#define XX(n)       \
  case LogLevel::LL_##n: \
    return #n
    XX(ERROR);
    XX(WARN);
    XX(INFO);
    XX(DEBUG);
    default:
      return "NT_UNKNOWN";
  }
#undef XX
}
LogLevel fromString(std::string level)
{

  if (level.size() < 2) {
    int num = 0;

    std::istringstream ss(level);
    ss >> num;
    return (LogLevel) num;
  }

  std::transform(level.begin(), level.end(), level.begin(), ::toupper);
#define XX(n)      \
  if (level == #n) \
  return LogLevel::LL_##n
  XX(ERROR);
  XX(WARN);
  XX(INFO);
  XX(DEBUG);
  return LogLevel::LL_UNKNOWN;
#undef XX
}

LogEvent::LogEvent(uint64_t timestamp, std::string file, int line, std::string logger_name, LogLevel level, std::string thread_name, int thread_id, int fiber_id, std::string func_name)
    : m_timestamp(timestamp),
      m_fileName(std::move(file)),
      m_line(line),
      m_loggerName(std::move(logger_name)),
      m_level(level),
      m_threadName(std::move(thread_name)),
      m_threadId(thread_id),
      m_fiberId(fiber_id),
      m_funcName(std::move(func_name)) {}

}// namespace myai
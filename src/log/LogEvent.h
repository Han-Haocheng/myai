//
// Created by HanHaocheng on 2023/11/4.
//

#ifndef MY_AI_LOGEVENT_H
#define MY_AI_LOGEVENT_H

#include <memory>
#include <sstream>
#include <string>

namespace myai
{
enum class LogLevel
{
  LL_UNKNOWN,
  LL_DEBUG,
  LL_INFO,
  LL_WARN,
  LL_ERROR,
};

std::string toString(LogLevel level);
LogLevel fromString(std::string level);

class LogEvent
{
public:
  typedef std::shared_ptr<LogEvent> ptr;

  LogEvent(uint64_t timestamp, std::string file, int line, std::string logger_name, LogLevel level, std::string thread_name, int thread_id, int fiber_id, std::string func_name);

  uint64_t getTimestamp() const { return m_timestamp; }
  LogLevel getLevel() const { return m_level; }
  const std::string &getFileName() const { return m_fileName; }
  int getLine() const { return m_line; }
  const std::string &getLoggerName() const { return m_loggerName; }
  const std::string &getThreadName() const { return m_threadName; }
  int getThreadId() const { return m_threadId; }
  int getFiberId() const { return m_fiberId; }
  const std::string &getFuncName() const
  {
    return m_funcName;
  }
  std::stringstream &msg() { return m_msg; }

private:
  uint64_t m_timestamp = 0;//时间戳
  std::string m_fileName{};//文件
  int m_line = 0;          //行数
  std::string m_loggerName{};

  LogLevel m_level = LogLevel::LL_UNKNOWN;

  std::string m_threadName{};
  int m_threadId = 0;    //线程id
  int m_fiberId = 0;     //协程id
  std::string m_funcName;//

  std::stringstream m_msg;
};

}// namespace myai

#endif//MY_AI_LOGEVENT_H

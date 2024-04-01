//
// Created by HanHaocheng on 2024/3/13.
//

#ifndef MY_AI_LOGEVENT_H
#define MY_AI_LOGEVENT_H

#include "../define/type.h"
#include <memory>
#include <sstream>
#include <utility>

MYLIB_BEGIN

enum class LogLevel : int8
{
  LL_UNKNOWN = -1,
  LL_DEBUG,
  LL_INFO,
  LL_WARN,
  LL_ERROR,
};

struct LogInfo {
  LogLevel level = LogLevel::LL_UNKNOWN;
  uint32 threadId = 0; //线程id
  uint32 fiberId = 0;  //协程id
  uint32 lineNum = 0;  //行数
  uint64 timestamp = 0;//时间戳

  String fileName;  //文件
  String threadName;//线程名
  String funcName;  //函数名
  String loggerName;//日志器名

  LogInfo() = default;
  ~LogInfo() = default;

  LogInfo(LogLevel level, uint32 threadId, uint32 fiberId, uint32 lineNum, uint64 timestamp, String fileName,
          String threadName, String funcName, String loggerName)
      : level(level), threadId(threadId), fiberId(fiberId),
        lineNum(lineNum), timestamp(timestamp), fileName(std::move(fileName)),
        threadName(std::move(threadName)), funcName(std::move(funcName)), loggerName(std::move(loggerName)) {}
};

class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  using wk_ptr = std::weak_ptr<LogEvent>;

  LogEvent() = default;
  ~LogEvent() = default;

  std::stringstream &msg() { return m_msg; }

  void setLogInfo(const LogInfo &logInfo) { m_logInfo = logInfo; }
  const LogInfo &getLogInfo() { return m_logInfo; }

  void clear() {
    m_logInfo = LogInfo();
    m_msg.clear();
  }

private:
  LogInfo m_logInfo;
  std::stringstream m_msg;
};
MYLIB_END

#endif//MY_AI_LOGEVENT_H

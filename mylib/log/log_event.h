//
// Created by HanHaocheng on 2024/4/10.
//

#ifndef MYPROJECT_LOG_EVENT_H
#define MYPROJECT_LOG_EVENT_H
#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN
class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  enum Type
  {
    LE_UNDEFINED = -1,
    LE_DEBUG,
    LE_INFO,
    LE_WARN,
    LE_ERROR,
  };

  LogEvent(String loggerName,
           Type type,
           uint32 line,
           String file,
           String funcName,
           String className,
           time_t t,
           tid_t thread,
           cid_t coroutine,
           String threadName,
           String coroutineName);

  inline SString &msg() { return m_msg; }

  inline const String &getLoggerName() const { return m_logger_name; }
  inline void setLoggerName(const String &mLoggerName) { m_logger_name = mLoggerName; }
  inline Type getType() const { return m_type; }
  inline void setType(Type mType) { m_type = mType; }
  inline uint32 getLine() const { return m_line; }
  inline void setLine(uint32 mLine) { m_line = mLine; }
  inline const String &getFile() const { return m_file; }
  inline void setFile(const String &mFile) { m_file = mFile; }
  inline const String &getFuncName() const { return m_func_name; }
  inline void setFuncName(const String &mFuncName) { m_func_name = mFuncName; }
  inline const String &getClassName() const { return m_class_name; }
  inline void setClassName(const String &mClassName) { m_class_name = mClassName; }
  inline const time_t &getTime() const { return m_time; }
  inline void setTime(time_t mTime) { m_time = mTime; }
  inline tid_t getThreadId() const { return m_thread_id; }
  inline void setThreadId(tid_t mThreadId) { m_thread_id = mThreadId; }
  inline cid_t getCoroutineId() const { return m_coroutine_id; }
  inline void setCoroutineId(cid_t mCoroutineId) { m_coroutine_id = mCoroutineId; }

  inline const String &getThreadName() const { return m_thread_name; }
  inline void setMThreadName(const String &mThreadName) { m_thread_name = mThreadName; }
  inline const String &getCoroutineName() const { return m_coroutine_name; }
  inline void setMCoroutineName(const String &mCoroutineName) { m_coroutine_name = mCoroutineName; }

private:
  // 输出类型
  String m_logger_name;      //日志器名
  Type m_type = LE_UNDEFINED;//日志类型

  // 位置信息
  uint32 m_line = 0u; //行数
  String m_file;      //文件名
  String m_func_name; //函数名
  String m_class_name;//类名

  // 异步信息
  time_t m_time = 0;       //时间戳
  tid_t m_thread_id = 0;   //线程id
  cid_t m_coroutine_id = 0;//协程id
  String m_thread_name;
  String m_coroutine_name;

  // 日志信息
  SString m_msg;
};

MYLIB_SPACE_END
#endif//MYPROJECT_LOG_EVENT_H

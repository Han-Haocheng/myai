//
// Created by HanHaocheng on 2024/4/10.
//

#include "log_event.h"

#include <utility>

MYLIB_SPACE_BEGIN

LogEvent::LogEvent(String appenderName,
                   String loggerName,
                   LogEvent::Type type,
                   uint32 line,
                   String file,
                   String funcName,
                   String className,
                   time_t t,
                   tid_t thread,
                   cid_t coroutine,
                   String threadName,
                   String coroutineName)
    : m_appender_name(std::move(appenderName)),
      m_logger_name(std::move(loggerName)),
      m_type(type),
      m_line(line),
      m_file(std::move(file)),
      m_func_name(std::move(funcName)),
      m_class_name(std::move(className)),
      m_time(t),
      m_thread_id(thread),
      m_coroutine_id(coroutine),
      m_thread_name(std::move(threadName)),
      m_coroutine_name(std::move(coroutineName)) {
}
MYLIB_SPACE_END

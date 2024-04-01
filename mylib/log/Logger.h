//
// Created by HanHaocheng on 2024/3/13.
//

#ifndef MY_AI_LOGGER_H
#define MY_AI_LOGGER_H

#include "../define/type.h"
#include "LogEvent.h"
#include "LogFormatter.h"
#include <vector>

MYLIB_BEGIN

class Logger {
public:
    Logger();
    ~Logger();

    void log(LogEvent::ptr event);
    void log(LogEvent::ptr event, const char *fmt, ...);
private:
  std::vector<LogAppender::ptr>m_appenders;
  LogFormatter::ptr m_formatter = nullptr;
};

MYLIB_END

#endif//MY_AI_LOGGER_H

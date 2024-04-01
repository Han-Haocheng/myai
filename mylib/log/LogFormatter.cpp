//
// Created by HanHaocheng on 2024/3/13.
//

#include "LogFormatter.h"

#include "LogEvent.h"
#include <iostream>
#include <utility>

MYLIB_BEGIN

class DefaultFormatter final : private LogFormatter::FormatterItem {
public:
  void format(SString &msg, const LogInfo &log_info) override {
    msg << m_exInfo;
  }
};

class LevelFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit LevelFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << (uint16)log_info.level; }
};
class LoggerNameFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit LoggerNameFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.loggerName; }
};
class ThreadIdFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit ThreadIdFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.threadId; }
};
class NewLineFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit NewLineFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << "\n"; }
};
class DateTimeFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit DateTimeFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {
    if (m_exInfo.empty()) {
      m_exInfo = "%Y-%m-%d %H:%M:%S";
    }
  }

  void format(SString &in_ss, const LogInfo &log_info) override {
    tm tm{};
    auto t = (time_t) log_info.timestamp;
#if __linux__
    localtime_r(&t, &tm);
#elif _WIN32
    localtime_s(&tm, &t);
#endif// __linux__
    char buf[64];
    strftime(buf, 64, m_exInfo.c_str(), &tm);
    in_ss << buf;
  }
};
class FileNameFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit FileNameFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.fileName; }
};
class LineFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit LineFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.lineNum; }
};
class TabFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit TabFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << "\t"; }
};
class FiberIdFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit FiberIdFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.fiberId; }
};
class ThreadNameFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit ThreadNameFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.threadName; }
};
class FuncNameFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit FuncNameFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << log_info.funcName; }
};
class PercentSignsFormatterItem final : public LogFormatter::FormatterItem {
public:
  explicit PercentSignsFormatterItem(String exFormatInfo) : FormatterItem(std::move(exFormatInfo)) {}
  void format(SString &in_ss, const LogInfo &log_info) override { in_ss << "%"; }
};

/*
XX(m, MassageFormatterItem),   // m : 消息
XX(p, LevelFormatterItem),     // p : 日志级别
XX(c, LoggerNameFormatterItem),// c : 日志名称
XX(t, ThreadIdFormatterItem),  // t : 线程id
XX(n, NewLineFormatterItem),   // n : 换行
XX(d, DateTimeFormatterItem),  // d : 时间
XX(f, FileNameFormatterItem),  // f : 文件名
XX(l, LineFormatterItem),      // l : 行号
XX(T, TabFormatterItem),       // T : 制表符
XX(F, FiberIdFormatterItem),   // F : 协程id
XX(N, ThreadNameFormatterItem),// N : 线程名字
XX(M, FuncNameFormatterItem),  // M : 方法名字
 */
const LogFormatter::FormatterItemMap LogFormatter::s_itemMap = {
#define XX(str, C)                                                                       \
  {                                                                                      \
    #str, [](const String &fmt) { return LogFormatter::FormatterItem::ptr{new C(fmt)}; } \
  }
    XX(p, LevelFormatterItem),     // p : 日志级别
    XX(c, LoggerNameFormatterItem),// c : 日志名称
    XX(t, ThreadIdFormatterItem),  // t : 线程id
    XX(n, NewLineFormatterItem),   // n : 换行
    XX(d, DateTimeFormatterItem),  // d : 时间
    XX(f, FileNameFormatterItem),  // f : 文件名
    XX(l, LineFormatterItem),      // l : 行号
    XX(T, TabFormatterItem),       // T : 制表符
    XX(F, FiberIdFormatterItem),   // F : 协程id
    XX(N, ThreadNameFormatterItem),// N : 线程名字
    XX(M, FuncNameFormatterItem),  // M : 方法名字
#undef XX
};

LogFormatter::LogFormatter(String pattern) : m_pattern(std::move(pattern)) {
  try {
    parse_pattern();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void LogFormatter::parse_pattern() {
  m_items;
}

MYLIB_END

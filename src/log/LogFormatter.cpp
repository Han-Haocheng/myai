//
// Created by HanHao*cheng on 2023/11/4.
//

#include "LogFormatter.h"

#include <iostream>
#include <utility>

namespace myai
{

LogFormatter::LogFormatterItem::LogFormatterItem(std::string exFormatInfo) : m_exString(std::move(exFormatInfo)) {}

class DefaultLogFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit DefaultLogFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << m_exString; }
};

class MassageFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit MassageFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->msg().str(); }
};

class LevelFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LevelFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << toString(event->getLevel()); }
};

class LoggerNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LoggerNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getLoggerName(); }
};

class ThreadIdFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit ThreadIdFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getThreadId(); }
};

class NewLineFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit NewLineFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "\n"; }
};

class DateTimeFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit DateTimeFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo))
  {
    if (m_format.empty()) {
      m_format = "%Y-%m-%d %H:%M:%S";
    }
  }
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override
  {
    tm tm{};
    auto t = (time_t) event->getTimestamp();
    localtime_r(&t, &tm);
    char buf[64];
    strftime(buf, 64, m_format.c_str(), &tm);
    in_ss << buf;
  }

private:
  std::string m_format;
};

class FileNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FileNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFileName(); }
};

class LineFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LineFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getLine(); }
};

class TabFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit TabFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "\t"; }
};

class FiberIdFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FiberIdFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFiberId(); }
};

class ThreadNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit ThreadNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getThreadName(); }
};

class FuncNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FuncNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFuncName(); }
};

class PercentSignsFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit PercentSignsFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "%"; }
};

std::map<std::string, std::function<LogFormatter::LogFormatterItem::ptr(const std::string &str)>> LogFormatter::s_item_map{
#define XX(str, C)                                                                 \
  {                                                                                \
    #str, [](const std::string &fmt) { return LogFormatterItem::ptr(new C(fmt)); } \
  }
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
#undef XX
};

//===================================================
LogFormatter::LogFormatter(std::string pattern)
    : m_pattern(std::move(pattern))
{
  if (m_pattern.empty()) {
    m_pattern = "";
  }
  parsePattern();
}

const std::string &LogFormatter::getPattern() const
{
  return m_pattern;
}
void LogFormatter::setPattern(const std::string &pattern)
{
  m_pattern = pattern;
  parsePattern();
}

std::string LogFormatter::format(const LogEvent::ptr &event)
{
  std::stringstream ss;
  for (const auto &item: m_items) {
    item->formatItem(ss, event);
  }
  return ss.str();
}

void LogFormatter::parsePattern()
{
  if (!m_items.empty()) {
    m_items.clear();
  }
  enum State
  {
    DEF_STR,
    FMT_STR,
    FMT_EX,
  } str_state = DEF_STR;

  struct Infos {
    State state;
    std::string str;
    std::string ex_str;
  };

  std::vector<Infos> item_strs;
  Infos infos_buf{DEF_STR};

  str_state = DEF_STR;

  for (auto ch = m_pattern.begin(); ch != m_pattern.end();) {
    if (str_state == DEF_STR) {
      if (*ch == '%') {

        if (!infos_buf.str.empty()) {
          item_strs.emplace_back(infos_buf);
        }
        infos_buf = {FMT_STR};
        str_state = FMT_STR;
      } else {
        infos_buf.str.push_back(*ch);
      }
    } else if (str_state == FMT_STR) {
      if (*ch == '{') {
        infos_buf.state = FMT_EX;
        str_state = FMT_EX;
      } else if (isalnum(*ch) || *ch == '.') {
        infos_buf.str.push_back(*ch);
      } else {
        if (!infos_buf.str.empty()) {
          item_strs.emplace_back(infos_buf);
        }
        infos_buf = {DEF_STR};
        str_state = DEF_STR;
        continue;
      }
    } else if (str_state == FMT_EX) {
      if (*ch == '}') {
        item_strs.emplace_back(infos_buf);
        infos_buf = {DEF_STR};
        str_state = DEF_STR;
      } else {
        infos_buf.ex_str.push_back(*ch);
      }
    }
    ++ch;
  }

  if (str_state == FMT_STR) {
    item_strs.push_back(infos_buf.str.empty() ? Infos{DEF_STR, "%", ""} : infos_buf);
  } else if (str_state == FMT_EX) {
    auto &b = item_strs.back();
    b.state = DEF_STR;
    std::cout << "<error LogFormatter::parsePattern> set fmt failed, fmt=`" << b.str << "`ex:`" << b.ex_str << "`"<< std::endl;
  } else {
    item_strs.emplace_back(infos_buf);
  }

  LogFormatterItem::ptr tmpPtr;
  for (const auto &infos: item_strs) {
    if (infos.state == DEF_STR) {
      m_items.emplace_back(new DefaultLogFormatterItem{infos.str});
    } else {
      auto findRt = s_item_map.find(infos.str);
      if (findRt == s_item_map.end()) {
        std::cout << "<error LogFormatter::parsePattern> find fmt:`" << infos.str << "`ex:`" << infos.ex_str << "`" << std::endl;
      }
      m_items.emplace_back(findRt->second(infos.ex_str));
    }
  }
}

}// namespace myai
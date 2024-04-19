//
// Created by HanHaocheng on 2024/4/10.
//

#include "log_formatter.h"
#include "../converter/converter.h"

#include <iostream>
#include <utility>
MYLIB_SPACE_BEGIN

template<>
class StringConverter<LogEvent::Type> {
public:
  using Ty = LogEvent::Type;
  static String toString(const Ty &ty) {
    switch (ty) {
      case Ty::LE_DEBUG:
        return "Debug";
      case Ty::LE_INFO:
        return "Info";
      case Ty::LE_WARN:
        return "Warn";
      case Ty::LE_ERROR:
        return "Error";
      default:
        return "Unknown";
    }
  }

  static Ty fromString(const String &ty) {
    static std::unordered_map<String, Ty> s_map{
#define XX(ABC, Abc, abc) {"LE_" #ABC, Ty::LE_##ABC}, {#ABC, Ty::LE_##ABC}, {#Abc, Ty::LE_##ABC}, {#abc, Ty::LE_##ABC}
        XX(DEBUG, Debug, debug),
        XX(INFO, Info, info),
        XX(WARN, Warn, warn),
        XX(ERROR, Error, error),
#undef XX
    };
    try {
      return s_map.at(ty);
    } catch (std::out_of_range &) {
      return Ty::LE_UNDEFINED;
    }
  }
};

// 字符串
class StringFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit StringFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return m_extendStr; }
};

class MessageFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit MessageFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return event->msg().str(); }
};

class TypeFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit TypeFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return mylib::StringConverter<mylib::LogEvent::Type>::toString(event->getType()); }
};

class ElapseFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit ElapseFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return ""; }
};

class LoggerNameFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit LoggerNameFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return event->getLoggerName(); }
};

class FileFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit FileFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return event->getFile(); }
};

class LineFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit LineFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return std::to_string(event->getLine()); }
};

class NewLineFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit NewLineFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return "\n"; }
};
class TabFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit TabFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return "\t"; }
};

// 异步相关
//时间
class DateTimeFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit DateTimeFormatItem(String extendStr = "%Y-%m-%d %H-%M-%S") : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override {
    char tmp[64];
    strftime(tmp, sizeof(tmp), m_extendStr.c_str(), localtime(&event->getTime()));
    return tmp;
  }
};

// 线程id
class ThreadIdFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit ThreadIdFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return std::to_string(event->getThreadId()); }
};

class ThreadNameFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit ThreadNameFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return event->getThreadName(); }
};

// 协程
class CoroutineIdFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit CoroutineIdFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return std::to_string(event->getCoroutineId()); }
};
class CoroutineNameFormatItem : public LogFormatter::LogFormatterItem {
public:
  explicit CoroutineNameFormatItem(String extendStr) : LogFormatterItem(std::move(extendStr)) {}
  [[nodiscard]] String format(LogEvent::ptr event) const override { return event->getCoroutineName(); }
};

LogFormatter::ItemMap LogFormatter::s_item_map = {};
LogFormatter::ptr LogFormatter::s_def_formatter = nullptr;

// 位置相关
// 文件
// 行数
//
//%d{%Y-%m-%d %H-%M-%S}[%f:%l]%T%p%T%c%T%t%T%m%n
LogFormatter::LogFormatter(mylib::String pattern) : m_pattern(std::move(pattern)) {
  if (s_item_map.empty()) {
    s_item_map = ItemMap{
#define XX(str, C)                                                                                    \
  {                                                                                                   \
    "%" #str, [](String exStr) { return LogFormatterItem::ptr{new C##FormatItem{std::move(exStr)}}; } \
  }
        XX(m, Message),    //m:消息
        XX(p, Type),       //p:日志级别
        XX(r, Elapse),     //r:累计毫秒数
        XX(c, LoggerName), //c:日志名称
        XX(t, ThreadId),   //t:线程id
        XX(n, NewLine),    //n:换行
        XX(d, DateTime),   //d:时间
        XX(f, File),       //f:文件名
        XX(l, Line),       //l:行号
        XX(T, Tab),        //T:Tab
        XX(F, CoroutineId),//F:协程id
        XX(N, ThreadName), //N:线程名称
#undef XX
    };
  }
  _parse_pattern();
}

void LogFormatter::setPattern(const String &pattern) {
  m_pattern = pattern;
  _parse_pattern();
}
String LogFormatter::format(const LogEvent::ptr &event) {
  SString res{};
  for (const auto &item: m_items) {
    String str = item->format(event);
    res << str;
  }
  return res.str();
}

void LogFormatter::_parse_pattern() {
  struct TmpFormat {
    String pattern;
    String extend;
    TmpFormat() = default;
    TmpFormat(String pattern, String extend) : pattern(std::move(pattern)), extend(std::move(extend)) {}
    [[nodiscard]] bool empty() const { return pattern.empty() && extend.empty(); };
  };

  String pattern = m_pattern;
  std::vector<TmpFormat> tmp_format_vec;
  TmpFormat tmp_format;

  for (size_t pos = 0; pos < m_pattern.size(); ++pos) {
    if (pattern[pos] != '%') {
      tmp_format.extend += pattern[pos];
      continue;
    }

    //检测复合格式化要求
    if (pos + 1 == m_pattern.size()) {
      tmp_format.extend += "%";
      continue;
    }
    if (!tmp_format.empty()) {
      tmp_format_vec.emplace_back(tmp_format);
      tmp_format = {};
    }

    //进入格式化模式
    tmp_format_vec.emplace_back((char[2]){pattern[pos++], pattern[pos]}, String{});

    // 检测是否符合扩展要求
    if (pattern[pos + 1] != '{') {
      //推出格式化模式
      continue;
    }

    auto &back_tf = tmp_format_vec.back();
    auto ex_end = pattern.find_first_of('}', pos + 2);
    if (ex_end == std::string::npos) {
      // 如果是扩展未闭合，则进入普通模式
      //pos = end_pos;
      continue;
    }
    back_tf.extend = pattern.substr(pos + 2, ex_end - pos - 2);
    pos = ex_end;
  }
  if (!tmp_format.empty()) {
    tmp_format_vec.emplace_back(tmp_format);
  }

  for (const auto &format: tmp_format_vec) {
    if (format.pattern.empty()) {
      m_items.emplace_back(new StringFormatItem(format.extend));
      continue;
    }
    auto fd_res = s_item_map.find(format.pattern);
    if (fd_res == s_item_map.end()) {
      m_items.emplace_back(new StringFormatItem(format.pattern + "{" + format.extend + "}"));
      continue;
    }
    m_items.emplace_back(fd_res->second(format.extend));
    for (const auto &item: m_items) {
    }
  }
}

MYLIB_SPACE_END

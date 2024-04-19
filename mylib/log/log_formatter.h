//
// Created by HanHaocheng on 2024/4/10.
//

#ifndef MYPROJECT_LOG_FORMATTER_H
#define MYPROJECT_LOG_FORMATTER_H
#include <utility>

#include "log_event.h"

MYLIB_SPACE_BEGIN

#define MYLIB_DEF_LOG_FORMATTER_PATTERN "[%d{%Y-%m-%d %H-%M-%S}][%f:%l]%T%p%T%c%T%t%T%m%n"
#define MYLIB_DEF_LOG_FORMATTER MYLIB_SPACE_NAME::LogFormatter::DefFormatter()


class LogFormatter {
public:
  class LogFormatterItem {
  public:
    using ptr = std::shared_ptr<LogFormatterItem>;
    explicit LogFormatterItem(String extendStr) : m_extendStr(std::move(extendStr)) {}
    [[nodiscard]] virtual String format(LogEvent::ptr event) const = 0;

  protected:
    String m_extendStr;//扩展
  };

  using ptr = std::shared_ptr<LogFormatter>;
  using FactoryFunc = std::function<LogFormatterItem::ptr(String extendStr)>;
  using ItemMap = std::unordered_map<String, FactoryFunc>;

public:
  explicit LogFormatter(String pattern);
  void setPattern(const String &pattern);
  String format(const LogEvent::ptr &event);

  static LogFormatter::ptr DefFormatter() {
    if (!s_def_formatter) {
      s_def_formatter.reset(new LogFormatter{MYLIB_DEF_LOG_FORMATTER_PATTERN});
    }
    return s_def_formatter;
  };

private:
  void _parse_pattern();

  String m_pattern;
  std::vector<LogFormatterItem::ptr> m_items;
  static ItemMap s_item_map;
  static LogFormatter::ptr s_def_formatter;
};

MYLIB_SPACE_END

#endif//MYPROJECT_LOG_FORMATTER_H

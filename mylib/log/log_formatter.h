//
// Created by HanHaocheng on 2024/4/10.
//

#ifndef MYPROJECT_LOG_FORMATTER_H
#define MYPROJECT_LOG_FORMATTER_H
#include <utility>

#include "log_event.h"

MYLIB_SPACE_BEGIN

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

private:
  void _parse_pattern();

  String m_pattern;
  std::vector<LogFormatterItem::ptr> m_items;
  static ItemMap s_item_map;
};

MYLIB_SPACE_END

#endif//MYPROJECT_LOG_FORMATTER_H

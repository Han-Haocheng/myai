//
// Created by HanHaocheng on 2024/3/13.
//

#ifndef MY_AI_LOGFORMATTER_H
#define MY_AI_LOGFORMATTER_H

#include "../define/type.h"
#include "LogEvent.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
MYLIB_BEGIN

class LogFormatter {
public:
  class FormatterItem {
  public:
    using ptr = std::shared_ptr<FormatterItem>;

    explicit FormatterItem(String exInfo) : m_exInfo(std::move(exInfo)) {}
    virtual void format(std::stringstream &msg, const LogInfo &log_info) = 0;

  protected:
    String m_exInfo;
  };
  using ptr = std::shared_ptr<LogFormatter>;
  using wkptr = std::weak_ptr<LogFormatter>;
  using FormatterItemMap = std::unordered_map<String, std::function<FormatterItem::ptr(const std::string &str)>>;
  explicit LogFormatter(String pattern);

private:
  void parse_pattern();

private:
  String m_pattern;
  std::vector<FormatterItem::ptr> m_items;
  static const FormatterItemMap s_itemMap;
};

MYLIB_END

#endif//MY_AI_LOGFORMATTER_H

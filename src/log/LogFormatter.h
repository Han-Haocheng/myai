//
// Created by HanHaocheng on 2023/11/4.
//

#ifndef MY_AI_LOGFORMATTER_H
#define MY_AI_LOGFORMATTER_H

#include "LogEvent.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace myai
{

// m : 消息
// p : 日志级别
// c : 日志名称
// t : 线程id
// n : 换行
// d : 时间
// f : 文件名
// l : 行号
// T : 制表符
// F : 协程id
// N : 线程名字
// M : 方法名字
class LogFormatter
{
public:
  typedef std::shared_ptr<LogFormatter> ptr;

  class LogFormatterItem
  {
  public:
    typedef std::shared_ptr<LogFormatterItem> ptr;

    explicit LogFormatterItem(std::string exFormatInfo);
    virtual ~LogFormatterItem() = default;

    virtual void formatItem(std::stringstream &in_ss, LogEvent::ptr event) = 0;

  protected:
    std::string m_exString{};
  };

public:
  explicit LogFormatter(std::string pattern);

  const std::string &getPattern() const;
  void setPattern(const std::string &pattern);

  std::string format(const LogEvent::ptr &event);

private:
  void parsePattern();

private:
  std::string m_pattern{};
  std::vector<LogFormatterItem::ptr> m_items;
  static std::map<std::string, std::function<LogFormatterItem::ptr(const std::string &str)>> s_item_map;
};

}// namespace myai

#endif//MY_AI_LOGFORMATTER_H

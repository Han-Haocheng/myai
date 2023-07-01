//
// Created by HanHaocheng on 2023/11/5.
//

#ifndef MY_AI_LOGAPPENDER_H
#define MY_AI_LOGAPPENDER_H

#include "LogFormatter.h"
#include <fstream>
#include <memory>
namespace myai
{
/*enum class LogAppenderType
{
  APD_CONSOLE,
  APD_FILE,
};*/

class LogAppender
{
public:
  enum Type
  {
    APD_UNDEFINE,
    APD_CONSOLE,
    APD_FILE,
  };
  static std::string ToString(Type type);
  static Type FromString(std::string type);

  typedef std::shared_ptr<LogAppender> ptr;
  LogAppender(LogLevel appender_level, Type appender_type, LogFormatter::ptr formatter);
  virtual ~LogAppender() = default;
  virtual void append(LogEvent::ptr event) = 0;

  LogLevel getLevel() const { return m_level; }
  void setLevel(LogLevel level) { m_level = level; }
  Type getType() const { return m_type; }
  void setType(Type type) { m_type = type; }
  const LogFormatter::ptr &getFormatter() const { return m_formatter; }
  void setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }

protected:
  LogLevel m_level = LogLevel::DEBUG;
  Type m_type = Type::APD_UNDEFINE;
  LogFormatter::ptr m_formatter = nullptr;
};

class ConsoleAppender : public LogAppender
{
public:
  explicit ConsoleAppender(LogLevel appender_level, LogFormatter::ptr formatter = nullptr);
  ~ConsoleAppender() override = default;
  void append(LogEvent::ptr event) override;
};

class FileAppender : public LogAppender
{
public:
  FileAppender(LogLevel appender_level, std::string file_path, LogFormatter::ptr formatter = nullptr);
  ~FileAppender() override;
  void append(LogEvent::ptr event) override;

private:
  bool reopenFile();

private:
  std::string m_filePath;
  std::ofstream m_ofs;
};

}// namespace myai

#endif//MY_AI_LOGAPPENDER_H

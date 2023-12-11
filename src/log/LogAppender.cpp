//
// Created by HanHaocheng on 2023/11/5.
//

#include "LogAppender.h"

#include <iostream>
#include <utility>

namespace myai
{
LogAppender::LogAppender(LogLevel appender_level, Type appender_type, LogFormatter::ptr formatter)
    : m_level(appender_level), m_type(appender_type), m_formatter(std::move(formatter))
{
}
std::string LogAppender::ToString(LogAppender::Type type)
{
  switch (type) {
#define XX(name) \
  case name:     \
    return #name
    XX(APD_CONSOLE);
    XX(APD_FILE);
    default:
      return "UNDEFINE";
#undef XX
  }
}
LogAppender::Type LogAppender::FromString(const std::string& type)
{
  if (type == "CONSOLE" || type == "console" || type == "Console" || type == "ConsoleAppender" || type == "APD_CONSOLE" || type == "1") {
    return LogAppender::APD_CONSOLE;
  }

  if (type == "FILE" || type == "file" || type == "File" || type == "FileAppender" || type == "APD_FILE" || type == "2") {
    return LogAppender::APD_FILE;
  }

  return LogAppender::APD_UNDEFINE;
}
ConsoleAppender::ConsoleAppender(LogLevel appender_level, LogFormatter::ptr formatter) : LogAppender(appender_level, Type::APD_CONSOLE, std::move(formatter)) {}

void ConsoleAppender::append(LogEvent::ptr event)
{
  if (m_level > event->getLevel()) {
    return;
  }

  std::cout << m_formatter->format(event);
}

FileAppender::FileAppender(LogLevel appender_level, std::string file_path, LogFormatter::ptr formatter)
    : LogAppender(appender_level, Type::APD_CONSOLE, std::move(formatter)), m_filePath(std::move(file_path)) {}

void FileAppender::append(LogEvent::ptr event)
{
  if (m_level > event->getLevel()) {
    return;
  }
  if (reopenFile()) {
    m_ofs << m_formatter->format(event);
  }
}
FileAppender::~FileAppender()
{
  if (m_ofs.is_open()) m_ofs.close();
}
bool FileAppender::reopenFile()
{
  if (!m_ofs.is_open()) {
    m_ofs.open(m_filePath, std::ios::out | std::ios::app);
    if (!m_ofs.is_open()) {
      std::cout << "<error FileAppender::FileAppender> open file failed, file path is " << m_filePath << "." << std::endl;
      return false;
    }
  }
  return true;
}
}// namespace myai
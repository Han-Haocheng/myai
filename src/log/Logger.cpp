//
// Created by HanHaocheng on 2023/11/5.
//

#include "Logger.h"
#include "LoggerManager.h"
#include <algorithm>
#include <iostream>
#include <utility>
namespace myai
{
Logger::Logger(std::string name, LogLevel logger_level) : m_name(std::move(name)), m_level(logger_level)
{
}
Logger::~Logger() = default;

void Logger::log(const LogEvent::ptr &event)
{
  if (m_level > event->getLevel()) {
    return;
  }

  if (m_appenders.empty()) {
    std::cout << "<warning Logger::log>logger name=" << m_name << " do not have appender." << std::endl;
    return;
  }

  for (const auto &appender: m_appenders) {
    appender->append(event);
  }
}

void Logger::addAppender(const LogAppender::ptr &appender)
{
  if (appender == nullptr) {
    std::cout << "<error Logger::addAppender>add appender is null" << std::endl;
  }

  if (appender->getFormatter() == nullptr) {
    appender->setFormatter(m_formatter);
    std::cout << "<warning Logger::addAppender>add appender don't have formatter." << std::endl;
  }
  m_appenders.emplace_back(appender);
}
void Logger::delAppender(const LogAppender::ptr &appender)
{
  auto findRt = std::find(m_appenders.begin(), m_appenders.end(), appender);
  if (findRt != m_appenders.end()) {
    m_appenders.erase(findRt);
  }
}
void Logger::clearAppenders()
{
  m_appenders.clear();
}
LogWarp::LogWarp(Logger::ptr logger, LogEvent::ptr event)
    : m_logger(std::move(logger)), m_event(std::move(event))
{
}
LogWarp::~LogWarp()
{
  m_logger->log(m_event);
}
}// namespace myai
//
// Created by HanHaocheng on 2024/4/10.
//

#include "logger.h"

MYLIB_SPACE_BEGIN
Logger::ptr Logger::m_root_logger = nullptr;

void Logger::addAppender(LogAppender::ptr appender) {
  if (!appender) {
    return;
  }

  appender->setFormatter(m_formatter);
  m_appenders.emplace_back(appender);
}

void Logger::log(const LogEvent::ptr &event) {
  if (event->getType() < m_type) {
    return;
  }

  if (m_appenders.empty()) {
    std::cout << "logger<" << m_name << ">appender is empty" << std::endl;
    m_root_logger->log(event);
    return;
  }

  for (auto &appender: m_appenders) {
    appender->append(event);
  }
}

MYLIB_SPACE_END

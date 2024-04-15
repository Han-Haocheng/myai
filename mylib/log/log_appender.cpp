//
// Created by HanHaocheng on 2024/4/10.
//

#include "log_appender.h"
#include <fstream>
#include <iostream>
MYLIB_SPACE_BEGIN

void ConsoleAppender::append(const LogEvent::ptr &event) {
  if (event->getType() < m_type) return;

  std::cout << m_formatter->format(event) << std::endl;
}

void FileAppender::append(const LogEvent::ptr &event) {
  if (event->getType() < m_type) return;
  while (true) {
    if (fs.is_open()) break;
    fs.open(m_file_path, std::ios::app);
  }
  fs << m_formatter->format(event) << std::endl;
}

MYLIB_SPACE_END

//
// Created by HanHaocheng on 2024/4/12.
//

#include "logger_manager.h"

MYLIB_SPACE_BEGIN
LoggerManager::ptr LoggerManager::s_instance = nullptr;

LoggerManager::ptr LoggerManager::GetInstance() {
  if (!s_instance) {
    s_instance.reset(new LoggerManager());
    s_instance->m_def_formatter.reset(new LogFormatter(MYLIB_LOG_DEF_FORMATTER_PATTERN));
    s_instance->m_root_logger.reset(new Logger(MYLIB_LOG_DEF_ROOT_NAME,
                                               MYLIB_LOG_DEF_LEVEL,
                                               s_instance->m_def_formatter));
    s_instance->m_root_logger->addAppender(LogAppender::ptr{new ConsoleAppender(MYLIB_LOG_DEF_LEVEL, s_instance->m_def_formatter)});
    s_instance->_add_logger(s_instance->m_root_logger);
  }
  return s_instance;
}

void LoggerManager::addLogger(const Logger::ptr &logger) {
  if (!logger || logger->getName().empty()) {
    //TODO: <log>error
    return;
  }
  _add_logger(logger);
}

Logger::ptr LoggerManager::addLogger(const String &name, LogEvent::Type level, const LogFormatter::ptr &formatter) {
  if (name.empty()) {
    return nullptr;
  }
  Logger::ptr logger =
      std::make_shared<Logger>(name, level == -1 ? MYLIB_LOG_DEF_LEVEL : level, formatter ? formatter : m_def_formatter);
  _add_logger(logger);
  return logger;
}

Logger::ptr LoggerManager::getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  return _get_logger(name);
}

void LoggerManager::_add_logger(const Logger::ptr &logger) {
  auto is_res = m_loggers.insert(std::make_pair(logger->getName(), logger));
  if (!is_res.second) {
    is_res.first->second = logger;
  }
}

Logger::ptr LoggerManager::_get_logger(const String &name) {
  auto fd_res = m_loggers.find(name);
  if (fd_res == m_loggers.end()) {
    return nullptr;
  }
  return fd_res->second;
}

MYLIB_SPACE_END
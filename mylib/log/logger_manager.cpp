//
// Created by HanHaocheng on 2024/4/12.
//

#include "logger_manager.h"

MYLIB_SPACE_BEGIN
LoggerManager::ptr LoggerManager::s_instance = nullptr;

LoggerManager::ptr LoggerManager::GetInstance() {
  if (!s_instance) {
    s_instance.reset(new LoggerManager());
    s_instance->_add_logger(MYLIB_ROOT_LOGGER);
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

Logger::ptr LoggerManager::addLogger(const String &name, LogEvent::Type level, LogFormatter::ptr formatter) {
  if (name.empty()) {
    return nullptr;
  }

  Logger::ptr logger = std::make_shared<Logger>(name,
                                                level == -1 ? MYLIB_DEF_LOGGER_LEVEL : level,
                                                formatter ? formatter : std::move(m_def_formatter));
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

Logger::ptr LoggerManager::try_getLogger(const String &name) {
  if (name.empty()) {
    //TODO: <log>error
    return nullptr;
  }
  Logger::ptr log = _get_logger(name);
  if (!log) {
    log.reset(new Logger{name, MYLIB_DEF_LOGGER_LEVEL, MYLIB_DEF_LOG_FORMATTER});
    _add_logger(log);
  }
  return log;
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
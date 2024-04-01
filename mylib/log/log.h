//
// Created by HanHaocheng on 2023/12/21.
//

#ifndef MY_LIB_LOG_H
#define MY_LIB_LOG_H

#include "../config/config.h"
#include "../format/format.h"
#include "../time/timer.h"
#include <cassert>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/node/emit.h>

#include "../asyn/asyn.h"
#include "../define/type.h"

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define MYLIB_LOG_BEGIN MYLIB_BEGIN namespace log {
#define MYLIB_LOG_END \
  MYLIB_END           \
  }

#define MYLIB_LOG_BASE(logger, level)                   \
  if (logger->getLevel() <= level)                      \
  ::mylib::log::LogWarp{                                \
      logger, ::std::make_shared<mylib::log::LogEvent>( \
                  ::mylib::Timer::GetCurrentTime(),     \
                  __FILE__,                             \
                  __LINE__,                             \
                  logger->getName(),                    \
                  level,                                \
                  "nonameThread",                       \
                  ::mylib::Thread::GetId(),             \
                  ::mylib::Coroutine::GetId(),          \
                  ::std::string{__PRETTY_FUNCTION__})}  \
      .msg()

#define MYLIB_LL_DEBUG ::mylib::log::LogEvent::LL_DEBUG
#define MYLIB_LL_INFO ::mylib::log::LogEvent::LL_INFO
#define MYLIB_LL_WARN ::mylib::log::LogEvent::LL_WARN
#define MYLIB_LL_ERROR ::mylib::log::LogEvent::LL_ERROR

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(logger, MYLIB_LL_DEBUG)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(logger, MYLIB_LL_INFO)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(logger, MYLIB_LL_WARN)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(logger, MYLIB_LL_ERROR)

#define MYLIB_LOGGER_ROOT mylib::log::LoggerManager::GetInstance()->getRootLogger()
#define MYLIB_LOGGER_NAME(name) mylib::log::LoggerManager::GetInstance()->addLogger(name, MYLIB_LL_DEBUG)

MYLIB_LOG_BEGIN

//=====================================================================================================================
class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  enum Level
  {
    LL_UNKNOWN,
    LL_DEBUG,
    LL_INFO,
    LL_WARN,
    LL_ERROR,
  };

  static std::string toString(Level level);
  static Level fromString(std::string level);

  LogEvent(uint64_t timestamp, std::string file, int line, std::string logger_name, LogEvent::Level level, std::string thread_name, tid_t thread_id, int fiber_id, std::string func_name);

  uint64_t getTimestamp() const { return m_timestamp; }
  LogEvent::Level getLevel() const { return m_level; }
  const std::string &getFileName() const { return m_fileName; }
  int getLine() const { return m_line; }
  const std::string &getLoggerName() const { return m_loggerName; }
  const std::string &getThreadName() const { return m_threadName; }
  int getThreadId() const { return m_threadId; }
  int getFiberId() const { return m_fiberId; }
  const std::string &getFuncName() const { return m_funcName; }
  std::stringstream &msg() { return m_msg; }

private:
  uint64_t m_timestamp = 0;//时间戳
  std::string m_fileName{};//文件
  int m_line = 0;          //行数
  std::string m_loggerName{};

  LogEvent::Level m_level = LogEvent::Level::LL_UNKNOWN;

  std::string m_threadName{};
  int m_threadId = 0;    //线程id
  int m_fiberId = 0;     //协程id
  std::string m_funcName;//

  std::stringstream m_msg;
};

//=====================================================================================================================

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
class LogFormatter {
public:
  class LogFormatterItem {
  public:
    typedef std::shared_ptr<LogFormatterItem> ptr;

    explicit LogFormatterItem(std::string exFormatInfo);
    virtual ~LogFormatterItem() = default;

    virtual void formatItem(std::stringstream &in_ss, LogEvent::ptr event) = 0;

  protected:
    std::string m_exString{};
  };

  typedef std::shared_ptr<LogFormatter> ptr;
  typedef std::map<std::string, std::function<LogFormatterItem::ptr(const std::string &str)>> ItemMap;

public:
  explicit LogFormatter(std::string pattern);

  [[nodiscard]] const std::string &getPattern() const;
  void setPattern(const std::string &pattern);

  std::string format(const LogEvent::ptr &event);

private:
  void parse_pattern();

private:
  std::string m_pattern{};
  std::vector<LogFormatterItem::ptr> m_items;
  static std::unique_ptr<ItemMap> s_item_map;
};

//=====================================================================================================================

class LogAppender {
public:
  enum Type
  {
    APD_UNDEFINE,
    APD_CONSOLE,
    APD_FILE,
  };
  static std::string ToString(Type type);
  static Type FromString(const std::string &type);

  typedef std::shared_ptr<LogAppender> ptr;
  LogAppender(LogEvent::Level appender_level, Type appender_type, LogFormatter::ptr formatter);
  virtual ~LogAppender() = default;
  virtual void append(LogEvent::ptr event) = 0;

  [[nodiscard]] LogEvent::Level getLevel() const;
  void setLevel(LogEvent::Level level);
  [[nodiscard]] Type getType() const;
  [[nodiscard]] const LogFormatter::ptr &getFormatter() const;
  void setFormatter(const LogFormatter::ptr &formatter);

protected:
  LogEvent::Level m_level = LogEvent::Level::LL_DEBUG;
  Type m_type = Type::APD_UNDEFINE;
  LogFormatter::ptr m_formatter = nullptr;
};

class ConsoleAppender : public LogAppender {
public:
  explicit ConsoleAppender(LogEvent::Level appender_level, LogFormatter::ptr formatter = nullptr);
  ~ConsoleAppender() override = default;
  void append(LogEvent::ptr event) override;
};

class FileAppender : public LogAppender {
public:
  FileAppender(LogEvent::Level appender_level, std::string file_path, LogFormatter::ptr formatter = nullptr);
  ~FileAppender() override;
  void append(LogEvent::ptr event) override;

private:
  bool reopenFile();

private:
  std::string m_filePath;
  std::ofstream m_ofs;
};

//=====================================================================================================================
class Logger {
public:
  typedef std::shared_ptr<Logger> ptr;
  Logger(std::string name, LogEvent::Level logger_level);
  ~Logger();

  void log(const LogEvent::ptr &event);

  void addAppender(const LogAppender::ptr &appender);
  void delAppender(const LogAppender::ptr &appender);
  void clearAppenders();

  const std::string &getName() const { return m_name; }
  LogEvent::Level getLevel() const { return m_level; }
  void setName(const std::string &name) { m_name = name; }
  void setLevel(LogEvent::Level level) { m_level = level; }

  LogFormatter::ptr getFormatter() const { return m_formatter; }
  void setFormatter(LogFormatter::ptr formatter) { m_formatter = std::move(formatter); }

private:
  std::string m_name;
  LogEvent::Level m_level = LogEvent::Level::LL_UNKNOWN;
  LogFormatter::ptr m_formatter = nullptr;
  std::vector<LogAppender::ptr> m_appenders;
};

//=====================================================================================================================
struct LogConfigVal {
  struct AppenderConfigVal {
    LogAppender::Type type;
    std::string file;
    LogEvent::Level level;
    std::string pattern;

    LogAppender::ptr createAppender(LogEvent::Level def_level, LogFormatter::ptr def_format) const;
  };

  struct LoggerConfigVal {
    std::string name;
    LogEvent::Level level;
    std::string pattern;
    std::vector<AppenderConfigVal> appenders;

    Logger::ptr createLogger(LogEvent::Level def_val, LogFormatter::ptr def_format) const;
  };

  LogEvent::Level def_level;
  std::string def_pattern;

  // LoggerConfigVal root_logger;
  std::vector<LoggerConfigVal> loggers;
};

///=====================================================================================================================

class LogWarp {
public:
  LogWarp(Logger::ptr logger, LogEvent::ptr event);
  ~LogWarp();
  std::stringstream &msg() { return m_event->msg(); }

private:
  Logger::ptr m_logger = nullptr;
  LogEvent::ptr m_event = nullptr;
};

///=====================================================================================================================

class LoggerManager {
public:
  typedef std::shared_ptr<LoggerManager> ptr;

  Logger::ptr setLogger(const std::string &name,
                        LogEvent::Level logger_level,
                        LogFormatter::ptr formatter = nullptr,
                        const std::vector<LogAppender::ptr> &appenders = {});
  bool setLogger(const Logger::ptr &logger);
  Logger::ptr getLogger(const std::string &name);
  Logger::ptr addLogger(const std::string &name, LogEvent::Level logger_level, LogFormatter::ptr formatter = nullptr, const std::vector<LogAppender::ptr> &appenders = {});
  bool addLogger(const Logger::ptr &logger);
  bool delLogger(const std::string &name);

  const Logger::ptr &getRootLogger() const;

  static LoggerManager::ptr GetInstance();
  LogEvent::Level getDefLevel() const;
  const LogFormatter::ptr &getDefFormatter() const;

private:
  explicit LoggerManager();

  // Logger::ptr config_logger(const LoggerConfigVal &conf);
  // static LogAppender::ptr config_appender(LogEvent::Level logger_level, LogFormatter::ptr logger_formatter, const AppenderConfigVal &conf);

private:
  Logger::ptr m_rootLogger = nullptr;
  std::unordered_map<std::string, Logger::ptr> m_loggers{};

  LogEvent::Level m_defLevel = LogEvent::LL_UNKNOWN;
  LogFormatter::ptr m_defFormatter = nullptr;
  static Config<LogConfigVal>::ptr s_log_configs;
};

//=====================================================================================================================
MYLIB_LOG_END

MYLIB_FORMAT_BEGIN

template<>
class Formatter<log::LogEvent::Level> {
public:
  using FormatType = log::LogEvent::Level;

  FormatType fromString(const std::string &type) {
    const static std::map<std::string, FormatType> FORMAT_MAP{
#define XX(NAME, name, Name, val)     \
  {#NAME, FormatType::LL_##NAME},     \
      {#val, FormatType::LL_##NAME},  \
      {#name, FormatType::LL_##NAME}, \
      {#Name, FormatType::LL_##NAME}

        XX(UNKNOWN, Unknown, unknown, 0),
        XX(DEBUG, Debug, debug, 1),
        XX(INFO, Info, info, 2),
        XX(WARN, Warn, warn, 3),
        XX(ERROR, Error, error, 4),
#undef XX
    };

    return FORMAT_MAP.at(type);
  }
  std::string toString(const FormatType &type) {
    switch (type) {
#define XX(NAME)                 \
  case log::LogEvent::LL_##NAME: \
    return #NAME
      XX(UNKNOWN);
      XX(DEBUG);
      XX(INFO);
      XX(WARN);
      XX(ERROR);
#undef XX
    }
    return "UNKNOWN";
  }
};

template<>
class Formatter<::mylib::log::LogAppender::Type> {
public:
  using FormatType = ::mylib::log::LogAppender::Type;

  FormatType fromString(const std::string &type) {
    if (type == "CONSOLE" || type == "console" || type == "Console" || type == "ConsoleAppender" || type == "APD_CONSOLE" || type == "1") {
      return FormatType::APD_CONSOLE;
    }
    if (type == "FILE" || type == "file" || type == "File" || type == "FileAppender" || type == "APD_FILE" || type == "2") {
      return FormatType::APD_FILE;
    }
    return FormatType::APD_UNDEFINE;
  }

  std::string toString(const FormatType &type) {
    switch (type) {
#define XX(name)               \
  case FormatType::APD_##name: \
    return #name
      XX(CONSOLE);
      XX(FILE);
      default:
        return "UNDEFINE";
#undef XX
    }
  }
};

template<>
class Formatter<log::LogConfigVal::AppenderConfigVal> {
public:
  using FormatType = log::LogConfigVal::AppenderConfigVal;

  FormatType fromString(const std::string &str) {
    YAML::Node node = YAML::Load(str);
    FormatType ret{};
    ret.type = Formatter<decltype(ret.type)>{}.fromString(node["type"].as<std::string>());
    if (!node["type"].IsDefined() || decltype(ret.type)::APD_UNDEFINE == ret.type) {
      std::cout << "<error ConfigCast<std::string, AppenderConfigVal>>appender type define bad." << std::endl;
    }

    if (ret.type == ::mylib::log::LogAppender::APD_FILE) {
      ret.file = node["file"].IsDefined() ? node["file"].as<std::string>() : "";
    }

    ret.level = node["level"].IsDefined()
                    ? ::mylib::log::LogEvent::fromString(node["level"].as<std::string>())
                    : ::mylib::log::LogEvent::Level::LL_UNKNOWN;
    ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
    return ret;
  }

  std::string toString(const FormatType &val) {
    YAML::Node node;

    if (val.type == ::mylib::log::LogAppender::APD_UNDEFINE) {
      throw std::runtime_error("Appender type is undefined.");
    }
    node["type"] = Formatter<decltype(val.type)>{}.toString(val.type);
    if (val.type == ::mylib::log::LogAppender::APD_FILE) {
      node["file"] = val.file.empty() ? "./log.txt" : val.file;
    }
    if (val.level != ::mylib::log::LogEvent::Level::LL_UNKNOWN) {
      node["level"] = ::mylib::log::LogEvent::toString(val.level);
    }
    if (!val.pattern.empty()) {
      node["pattern"] = val.pattern;
    }

    return YAML::Dump(node);
  }
};
template<>
class Formatter<log::LogConfigVal::LoggerConfigVal> {
public:
  using FormatType = log::LogConfigVal::LoggerConfigVal;
  FormatType fromString(const std::string &val) {
    YAML::Node node = YAML::Load(val);
    if (!node["name"].IsDefined()) {
      std::cout << "<error ConfigCast>LoggerConfigVal string undefine name";
    }
    FormatType ret{};

    ret.name = node["name"].as<std::string>();
    ret.level = node["level"].IsDefined()
                    ? ::mylib::log::LogEvent::fromString(node["level"].as<std::string>())
                    : ::mylib::log::LogEvent::Level::LL_UNKNOWN;
    ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
    if (node["appenders"].IsDefined()) {
      for (auto appender_node: node["appenders"]) {
        ret.appenders.emplace_back(Formatter<log::LogConfigVal::AppenderConfigVal>{}.fromString(YAML::Dump(appender_node)));
      }
    }
    return ret;
  }

  std::string toString(const FormatType &val) {
    YAML::Node node;
    if (!val.name.empty()) {
      node["name"] = val.name;
    }

    if (val.level != decltype(val.level)::LL_UNKNOWN) {
      node["level"] = ::mylib::log::LogEvent::toString(val.level);
    }
    if (!val.pattern.empty()) { node["pattern"] = val.pattern; }
    if (!val.appenders.empty()) {
      for (auto appender: val.appenders) {
        node["appenders"].push_back(YAML::Load(Formatter<log::LogConfigVal::AppenderConfigVal>{}.toString(appender)));
      }
    }
    return YAML::Dump(node);
  }
};

template<>
class Formatter<log::LogConfigVal> {
public:
  using FormatType = log::LogConfigVal;

  FormatType fromString(const std::string &val) {
    YAML::Node node = YAML::Load(val);
    FormatType ret{};

    ret.def_level = node["def_level"].IsDefined()
                        ? log::LogEvent::fromString(node["def_level"].as<std::string>())
                        : log::LogEvent::Level::LL_UNKNOWN;
    ret.def_pattern = node["def_pattern"].IsDefined()
                          ? node["def_pattern"].as<std::string>()
                          : "";
    auto logger_nodes = node["loggers"];
    if (logger_nodes.IsDefined() && logger_nodes.IsSequence()) {
      for (auto logger_node: logger_nodes) {
        ret.loggers.emplace_back(Formatter<FormatType::LoggerConfigVal>{}.fromString(YAML::Dump(logger_node)));
      }
    }
    return ret;
  }

  std::string toString(const FormatType &val) {
    YAML::Node node;
    node["def_level"] = log::LogEvent::toString(val.def_level);

    if (!val.def_pattern.empty()) {
      node["def_pattern"] = val.def_pattern;
    }
    //    node["root_logger"] = m_lcv.toString(val.root_logger);
    if (!val.loggers.empty()) {
      for (auto logger: val.loggers) {
        node["loggers"].push_back(Formatter<FormatType::LoggerConfigVal>{}.toString(logger));
      }
    }

    return YAML::Dump(node);
  }

private:
};
//=====================================================================================================================

MYLIB_FORMAT_END

#endif//MY_LIB_LOG_H

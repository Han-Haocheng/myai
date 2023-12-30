//
// Created by HanHaocheng on 2023/12/21.
//

#ifndef MY_LIB_LOG_H
#define MY_LIB_LOG_H

#include "asyn.h"
#include "config.h"
#include "timer.h"
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#define MYLIB_LOG_BASE(logger, level)                  \
  if (logger->getLevel() <= level)                     \
  ::mylib::LogWarp{                                    \
      logger, ::std::make_shared<mylib::LogEvent>(     \
                  ::mylib::Timer::GetCurrentTime(),    \
                  __FILE__,                            \
                  __LINE__,                            \
                  logger->getName(),                   \
                  level,                               \
                  "nonameThread",                      \
                  ::mylib::Thread::GetId(),            \
                  ::mylib::Coroutine::GetId(),         \
                  ::std::string{__PRETTY_FUNCTION__})} \
      .msg()

#define MYLIB_LOG_DEBUG(logger) MYLIB_LOG_BASE(logger, ::mylib::LogEvent::LL_DEBUG)
#define MYLIB_LOG_INFO(logger) MYLIB_LOG_BASE(logger, ::mylib::LogEvent::LL_INFO)
#define MYLIB_LOG_WARN(logger) MYLIB_LOG_BASE(logger, ::mylib::LogEvent::LL_WARN)
#define MYLIB_LOG_ERROR(logger) MYLIB_LOG_BASE(logger, ::mylib::LogEvent::LL_ERROR)

#define MYLIB_LOGGER_ROOT mylib::LoggerManager::GetInstance()->getRootLogger()
#define MYLIB_LOGGER_NAME(name) mylib::LoggerManager::GetInstance()->addLogger(name, ::mylib::LogEvent::LL_DEBUG)

namespace mylib
{
//=====================================================================================================================
class LogEvent
{
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

  LogEvent(uint64_t timestamp, std::string file, int line, std::string logger_name, LogEvent::Level level, std::string thread_name, int thread_id, int fiber_id, std::string func_name);

  inline uint64_t getTimestamp() const { return m_timestamp; }
  inline LogEvent::Level getLevel() const { return m_level; }
  inline const std::string &getFileName() const { return m_fileName; }
  inline int getLine() const { return m_line; }
  inline const std::string &getLoggerName() const { return m_loggerName; }
  inline const std::string &getThreadName() const { return m_threadName; }
  inline int getThreadId() const { return m_threadId; }
  inline int getFiberId() const { return m_fiberId; }
  inline const std::string &getFuncName() const { return m_funcName; }
  inline std::stringstream &msg() { return m_msg; }

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
class LogFormatter
{
public:
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

class ConsoleAppender : public LogAppender
{
public:
  explicit ConsoleAppender(LogEvent::Level appender_level, LogFormatter::ptr formatter = nullptr);
  ~ConsoleAppender() override = default;
  void append(LogEvent::ptr event) override;
};

class FileAppender : public LogAppender
{
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
class Logger
{
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
///=====================================================================================================================
/// AppenderConfigVal
///=====================================================================================================================

struct AppenderConfigVal {
  LogAppender::Type type;
  std::string file;
  LogEvent::Level level;
  std::string pattern;
};

///=====================================================================================================================
/// LoggerConfigVal
///=====================================================================================================================

struct LoggerConfigVal {
  std::string name;
  LogEvent::Level level;
  std::string pattern;
  std::vector<AppenderConfigVal> appenders;
  bool operator<(const LoggerConfigVal &rhs) const { return name < rhs.name; }
  bool operator>(const LoggerConfigVal &rhs) const { return rhs < *this; }
  bool operator<=(const LoggerConfigVal &rhs) const { return !(rhs < *this); }
  bool operator>=(const LoggerConfigVal &rhs) const { return !(*this < rhs); }
  bool operator==(const LoggerConfigVal &rhs) const { return name == rhs.name; }
  bool operator!=(const LoggerConfigVal &rhs) const { return !(rhs == *this); }
};

///=====================================================================================================================
/// LogConfigVal
///=====================================================================================================================

struct LogConfigVal {
  LogEvent::Level def_level;
  std::string def_pattern;

  LoggerConfigVal root_logger;
  std::set<LoggerConfigVal> loggers;
};

///=====================================================================================================================

class LogWarp
{
public:
  LogWarp(Logger::ptr logger, LogEvent::ptr event);
  ~LogWarp();
  std::stringstream &msg() { return m_event->msg(); }

private:
  Logger::ptr m_logger = nullptr;
  LogEvent::ptr m_event = nullptr;
};

///=====================================================================================================================

class LoggerManager
{
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
  Logger::ptr config_logger(const LoggerConfigVal &conf);
  static LogAppender::ptr config_appender(LogEvent::Level logger_level, LogFormatter::ptr logger_formatter, const AppenderConfigVal &conf);

private:
  Logger::ptr m_rootLogger = nullptr;
  std::vector<LoggerConfigVal> m_loggerConfigs{};
  std::unordered_map<std::string, Logger::ptr> m_loggers{};

  LogEvent::Level m_defLevel = LogEvent::LL_UNKNOWN;
  LogFormatter::ptr m_defFormatter = nullptr;
  static Config<LogConfigVal>::ptr s_log_configs;
};

//=====================================================================================================================
}// namespace mylib

#endif//MY_LIB_LOG_H

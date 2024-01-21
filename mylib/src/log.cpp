//
// Created by HanHaocheng on 2023/12/21.
//

#include "log.h"
#include <algorithm>
#include <iostream>

namespace mylib
{
struct AppenderConfigVal {
  LogAppender::Type type;
  std::string file;
  LogEvent::Level level;
  std::string pattern;
};

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

struct LogConfigVal {

  LogEvent::Level def_level;
  std::string def_pattern;

  // LoggerConfigVal root_logger;
  std::vector<LoggerConfigVal> loggers;
};

template<>
class Formatter<LogAppender::Type>
{
public:
  LogAppender::Type fromString(const std::string &type);
  std::string toString(const LogAppender::Type &type);
};
template<>
class Formatter<AppenderConfigVal>
{
public:
  AppenderConfigVal fromString(const std::string &str);
  std::string toString(const AppenderConfigVal &val);

private:
  Formatter<LogAppender::Type> m_lat;
};
template<>
class Formatter<LoggerConfigVal>
{
public:
  LoggerConfigVal fromString(const std::string &val);
  std::string toString(const LoggerConfigVal &val);

private:
  Formatter<std::vector<AppenderConfigVal>> m_acv_vec;
};
template<>
class Formatter<LogConfigVal>
{
public:
  LogConfigVal fromString(const std::string &val);
  std::string toString(const LogConfigVal &val);

private:
  // Formatter<LoggerConfigVal> m_lcv;
  Formatter<std::vector<LoggerConfigVal>> m_lcv_vec;
};

//=====================================================================================================================

Config<LogConfigVal>::ptr LoggerManager::s_log_configs = nullptr;
std::unique_ptr<LogFormatter::ItemMap> LogFormatter::s_item_map = nullptr;

//=====================================================================================================================

std::string Formatter<LogAppender::Type>::toString(const LogAppender::Type &type)
{
  switch (type) {
#define XX(name)                \
  case LogAppender::APD_##name: \
    return #name
    XX(CONSOLE);
    XX(FILE);
    default:
      return "UNDEFINE";
#undef XX
  }
}

LogAppender::Type Formatter<LogAppender::Type>::fromString(const std::string &type)
{
  if (type == "CONSOLE" || type == "console" || type == "Console" || type == "ConsoleAppender" || type == "APD_CONSOLE" || type == "1") {
    return LogAppender::APD_CONSOLE;
  }
  if (type == "FILE" || type == "file" || type == "File" || type == "FileAppender" || type == "APD_FILE" || type == "2") {
    return LogAppender::APD_FILE;
  }
  return LogAppender::APD_UNDEFINE;
}

std::string Formatter<AppenderConfigVal>::toString(const AppenderConfigVal &val)
{
  YAML::Node node;
  if (val.type == LogAppender::APD_UNDEFINE) { throw std::runtime_error("Appender type is undefined."); }
  node["type"] = m_lat.toString(val.type);
  if (val.type == LogAppender::APD_FILE) { node["file"] = val.file.empty() ? "./log.txt" : val.file; }
  if (val.level != LogEvent::Level::LL_UNKNOWN) { node["level"] = LogEvent::toString(val.level); }
  if (!val.pattern.empty()) { node["pattern"] = val.pattern; }
  return YAML::Dump(node);
}

AppenderConfigVal Formatter<AppenderConfigVal>::fromString(const std::string &str)
{
  YAML::Node node = YAML::Load(str);
  AppenderConfigVal ret{};
  ret.type = m_lat.fromString(YAML::Dump(node["type"]));
  if (!node["type"].IsDefined() || LogAppender::APD_UNDEFINE == ret.type) {
    std::cout << "<error ConfigCast<std::string, AppenderConfigVal>>appender type define bad." << std::endl;
  }

  if (ret.type == LogAppender::APD_FILE) {
    ret.file = node["file"].IsDefined() ? node["file"].as<std::string>() : "";
  }

  ret.level = node["level"].IsDefined() ? LogEvent::fromString(node["level"].as<std::string>()) : LogEvent::Level::LL_UNKNOWN;
  ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
  return ret;
}

std::string Formatter<LoggerConfigVal>::toString(const LoggerConfigVal &val)
{
  YAML::Node node;
  if (val.name.empty()) { throw std::runtime_error("Logger name is undefined."); }
  node["name"] = val.name;
  if (val.level != LogEvent::Level::LL_UNKNOWN) { node["level"] = LogEvent::toString(val.level); }
  if (!val.pattern.empty()) { node["pattern"] = val.pattern; }
  if (val.appenders.empty()) { node.push_back(YAML::Load(m_acv_vec.toString(val.appenders))); }
  return YAML::Dump(node);
}
LoggerConfigVal Formatter<LoggerConfigVal>::fromString(const std::string &val)
{
  YAML::Node node = YAML::Load(val);
  if (!node["name"].IsDefined()) {
    std::cout << "<error ConfigCast>LoggerConfigVal string undefine name";
  }
  LoggerConfigVal ret{};

  ret.name = node["name"].as<std::string>();
  ret.level = node["level"].IsDefined() ? LogEvent::fromString(node["level"].as<std::string>()) : LogEvent::Level::LL_UNKNOWN;
  ret.pattern = node["pattern"].IsDefined() ? node["pattern"].as<std::string>() : "";
  if (node["appenders"].IsDefined()) {
    ret.appenders = m_acv_vec.fromString(YAML::Dump(node["appenders"]));
  }

  return ret;
}

LogConfigVal Formatter<LogConfigVal>::fromString(const std::string &val)
{
  YAML::Node node = YAML::Load(val);
  LogConfigVal ret{};

  ret.def_level = node["def_level"].IsDefined()
                      ? LogEvent::fromString(node["def_level"].as<std::string>())
                      : LogEvent::Level::LL_UNKNOWN;
  ret.def_pattern = node["def_pattern"].IsDefined()
                        ? node["def_pattern"].as<std::string>()
                        : "";
  //    if (node["root_logger"].IsDefined()) {
  //      ret.root_logger = m_lcv.fromString(Dump(node["root_logger"]));
  //    }
  if (node["loggers"].IsDefined()) {
    ret.loggers = m_lcv_vec.fromString(Dump(node["loggers"]));
  }
  return ret;
}

std::string Formatter<LogConfigVal>::toString(const LogConfigVal &val)
{
  YAML::Node node;
  node["def_level"] = LogEvent::toString(val.def_level);

  if (!val.def_pattern.empty()) {
    node["def_pattern"] = val.def_pattern;
  }
  //    node["root_logger"] = m_lcv.toString(val.root_logger);
  if (!val.loggers.empty()) {
    node["loggers"] = m_lcv_vec.toString(val.loggers);
  }
  return YAML::Dump(node);
}

//=====================================================================================================================
std::string LogEvent::toString(LogEvent::Level level)
{
  switch (level) {
#define XX(n)                   \
  case LogEvent::Level::LL_##n: \
    return #n
    XX(ERROR);
    XX(WARN);
    XX(INFO);
    XX(DEBUG);
    default:
      return "LL_UNKNOWN";
  }
#undef XX
}

LogEvent::Level LogEvent::fromString(std::string level)
{

  if (level.size() < 2) {
    int num = 0;

    std::istringstream ss(level);
    ss >> num;
    return (LogEvent::Level) num;
  }

  std::transform(level.begin(), level.end(), level.begin(), ::toupper);
#define XX(n)      \
  if (level == #n) \
  return LogEvent::Level::LL_##n
  XX(ERROR);
  XX(WARN);
  XX(INFO);
  XX(DEBUG);
  return LogEvent::Level::LL_UNKNOWN;
#undef XX
}

LogEvent::LogEvent(uint64_t timestamp, std::string file, int line, std::string logger_name, LogEvent::Level level, std::string thread_name, tid_t thread_id, int fiber_id, std::string func_name)
    : m_timestamp(timestamp),
      m_fileName(std::move(file)),
      m_line(line),
      m_loggerName(std::move(logger_name)),
      m_level(level),
      m_threadName(std::move(thread_name)),
      m_threadId(thread_id),
      m_fiberId(fiber_id),
      m_funcName(std::move(func_name)) {}

//=====================================================================================================================

LogFormatter::LogFormatterItem::LogFormatterItem(std::string exFormatInfo) : m_exString(std::move(exFormatInfo)) {}
class DefaultLogFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit DefaultLogFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << m_exString; }
};
class MassageFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit MassageFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->msg().str(); }
};
class LevelFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LevelFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << LogEvent::toString(event->getLevel()); }
};
class LoggerNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LoggerNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getLoggerName(); }
};
class ThreadIdFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit ThreadIdFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getThreadId(); }
};
class NewLineFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit NewLineFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "\n"; }
};
class DateTimeFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit DateTimeFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo))
  {
    if (m_exString.empty()) {
      m_exString = "%Y-%m-%d %H:%M:%S";
    }
  }

  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override
  {
    tm tm{};
    auto t = (time_t) event->getTimestamp();
#if __linux__
    localtime_r(&t, &tm);
#elif _WIN32
    localtime_s(&tm, &t);
#endif // __linux__
    char buf[64];
    strftime(buf, 64, m_exString.c_str(), &tm);
    in_ss << buf;
  }
};
class FileNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FileNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFileName(); }
};
class LineFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit LineFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getLine(); }
};
class TabFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit TabFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "\t"; }
};
class FiberIdFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FiberIdFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFiberId(); }
};
class ThreadNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit ThreadNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getThreadName(); }
};
class FuncNameFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit FuncNameFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << event->getFuncName(); }
};
class PercentSignsFormatterItem : public LogFormatter::LogFormatterItem
{
public:
  explicit PercentSignsFormatterItem(std::string exFormatInfo) : LogFormatterItem(std::move(exFormatInfo)) {}
  void formatItem(std::stringstream &in_ss, LogEvent::ptr event) override { in_ss << "%"; }
};
//=====================================================================================================================
LogFormatter::LogFormatter(std::string pattern)
    : m_pattern(std::move(pattern))
{
  if (!s_item_map) {
    s_item_map.reset(new ItemMap{
#define XX(str, C)                                                                 \
  {                                                                                \
    #str, [](const std::string &fmt) { return LogFormatterItem::ptr(new C(fmt)); } \
  }
        XX(m, MassageFormatterItem),   // m : 消息
        XX(p, LevelFormatterItem),     // p : 日志级别
        XX(c, LoggerNameFormatterItem),// c : 日志名称
        XX(t, ThreadIdFormatterItem),  // t : 线程id
        XX(n, NewLineFormatterItem),   // n : 换行
        XX(d, DateTimeFormatterItem),  // d : 时间
        XX(f, FileNameFormatterItem),  // f : 文件名
        XX(l, LineFormatterItem),      // l : 行号
        XX(T, TabFormatterItem),       // T : 制表符
        XX(F, FiberIdFormatterItem),   // F : 协程id
        XX(N, ThreadNameFormatterItem),// N : 线程名字
        XX(M, FuncNameFormatterItem),  // M : 方法名字
#undef XX
    });
  }
  if (m_pattern.empty()) {
    m_pattern = "";
  }
  parse_pattern();
}

const std::string &LogFormatter::getPattern() const
{
  return m_pattern;
}
void LogFormatter::setPattern(const std::string &pattern)
{
  m_pattern = pattern;
  parse_pattern();
}

std::string LogFormatter::format(const LogEvent::ptr &event)
{
  if (m_items.empty()) {
    return "";
  }
  std::stringstream ss = std::stringstream();
  for (const auto &item: m_items) {
    item->formatItem(ss, event);
  }
  return ss.str();
}

void LogFormatter::parse_pattern()
{
  if (!m_items.empty()) {
    m_items.clear();
  }

  enum State
  {
    DEF_STR,
    FMT_STR,
    FMT_EX,
  };

  struct Infos {
    State state;
    std::string str;
    std::string ex_str;
  };

  std::vector<Infos> item_strs;
  Infos infos_buf{DEF_STR};
  State str_state = State::DEF_STR;

  for (auto ch = m_pattern.begin(); ch != m_pattern.end();) {
    if (str_state == DEF_STR) {
      if (*ch == '%') {

        if (!infos_buf.str.empty()) {
          item_strs.emplace_back(infos_buf);
        }
        infos_buf = {FMT_STR};
        str_state = FMT_STR;
      } else {
        infos_buf.str.push_back(*ch);
      }
    } else if (str_state == FMT_STR) {
      if (*ch == '{') {
        infos_buf.state = FMT_EX;
        str_state = FMT_EX;
      } else if (isalnum(*ch) || *ch == '.') {
        infos_buf.str.push_back(*ch);
      } else {
        if (!infos_buf.str.empty()) {
          item_strs.emplace_back(infos_buf);
        }
        infos_buf = {DEF_STR};
        str_state = DEF_STR;
        continue;
      }
    } else if (str_state == FMT_EX) {
      if (*ch == '}') {
        item_strs.emplace_back(infos_buf);
        infos_buf = {DEF_STR};
        str_state = DEF_STR;
      } else {
        infos_buf.ex_str.push_back(*ch);
      }
    }
    ++ch;
  }

  if (str_state == FMT_STR) {
    item_strs.push_back(infos_buf.str.empty() ? Infos{DEF_STR, "%", ""} : infos_buf);
  } else if (str_state == FMT_EX) {
    auto &b = item_strs.back();
    b.state = DEF_STR;
    std::cout << "<error LogFormatter::parse_pattern> set fmt failed, fmt=`" << b.str << "`ex:`" << b.ex_str << "`" << std::endl;
  } else {
    item_strs.emplace_back(infos_buf);
  }

  LogFormatterItem::ptr tmpPtr;
  for (const auto &infos: item_strs) {
    if (infos.state == DEF_STR) {
      m_items.emplace_back(new DefaultLogFormatterItem{infos.str});
    } else {
      auto findRt = s_item_map->find(infos.str);
      if (findRt == s_item_map->end()) {
        std::cout << "<error LogFormatter::parse_pattern> find fmt:`" << infos.str << "`ex:`" << infos.ex_str << "`" << std::endl;
      }
      m_items.emplace_back(findRt->second(infos.ex_str));
    }
  }
}
//=====================================================================================================================
LogAppender::LogAppender(LogEvent::Level appender_level, Type appender_type, LogFormatter::ptr formatter)
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

LogAppender::Type LogAppender::FromString(const std::string &type)
{
  if (type == "CONSOLE" || type == "console" || type == "Console" || type == "ConsoleAppender" || type == "APD_CONSOLE" || type == "1") {
    return LogAppender::APD_CONSOLE;
  }
  if (type == "FILE" || type == "file" || type == "File" || type == "FileAppender" || type == "APD_FILE" || type == "2") {
    return LogAppender::APD_FILE;
  }
  return LogAppender::APD_UNDEFINE;
}
LogEvent::Level LogAppender::getLevel() const { return m_level; }
void LogAppender::setLevel(LogEvent::Level level) { m_level = level; }
LogAppender::Type LogAppender::getType() const { return m_type; }
const LogFormatter::ptr &LogAppender::getFormatter() const { return m_formatter; }
void LogAppender::setFormatter(const LogFormatter::ptr &formatter) { m_formatter = formatter; }
ConsoleAppender::ConsoleAppender(LogEvent::Level appender_level, LogFormatter::ptr formatter)
    : LogAppender(appender_level, Type::APD_CONSOLE, std::move(formatter)) {}
void ConsoleAppender::append(LogEvent::ptr event)
{
  if (m_level <= event->getLevel())
    std::cout << m_formatter->format(event);
}

FileAppender::FileAppender(LogEvent::Level appender_level, std::string file_path, LogFormatter::ptr formatter)
    : LogAppender(appender_level, Type::APD_CONSOLE, std::move(formatter)), m_filePath(std::move(file_path)) {}
void FileAppender::append(LogEvent::ptr event)
{
  if (m_level < event->getLevel() && reopenFile()) m_ofs << m_formatter->format(event);
}
FileAppender::~FileAppender()
{
  if (m_ofs.is_open()) m_ofs.close();
}
bool FileAppender::reopenFile()
{
  if (m_ofs.is_open())
    return true;
  m_ofs.open(m_filePath, std::ios::out | std::ios::app);
  if (m_ofs.is_open()) {
    return true;
  }
  std::cout << "<error FileAppender::FileAppender> open file failed, file path is " << m_filePath << "." << std::endl;
  return false;
}
//=====================================================================================================================
Logger::Logger(std::string name, LogEvent::Level logger_level) : m_name(std::move(name)), m_level(logger_level)
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
    std::cout << "<warning Logger::addAppender>add appender don't have formatter." << std::endl;
    if (!m_formatter) {
      std::cout << "<error Logger::addAppender>m_formatter is null" << std::endl;
      return;
    }
    appender->setFormatter(m_formatter);
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
//=====================================================================================================================

LoggerManager::ptr LoggerManager::GetInstance()
{
  static LoggerManager::ptr manager{new LoggerManager{}};
  if (manager == nullptr) {
    manager.reset(new LoggerManager{});
  }
  return manager;
}

LoggerManager::LoggerManager()
{
  if (!s_log_configs) {
    s_log_configs = Configer::GetInstance()->setConfig<LogConfigVal>("log",
                                                                     {
                                                                         LogEvent::Level::LL_DEBUG,
                                                                         "%d{%Y-%m-%d %H:%M:%S} %m%n",
                                                                         {
                                                                             {"root",
                                                                              LogEvent::Level::LL_DEBUG,
                                                                              "%d{%Y-%m-%d %H:%M:%S} %t%T%F%T%N [%p] [%c] %f:%l %M ==> %m%n",
                                                                              {{LogAppender::Type::APD_CONSOLE,
                                                                                "",
                                                                                LogEvent::Level::LL_DEBUG,
                                                                                ""}}},
                                                                         },
                                                                     },
                                                                     "log");
    s_log_configs->addListener([this](const LogConfigVal &old_confs, const LogConfigVal &new_confs) {
      m_defLevel = new_confs.def_level;
      m_defFormatter->setPattern(new_confs.def_pattern);

      //Check if the new configuration is included in the old configuration
      for (const auto &new_conf: new_confs.loggers) {
        auto fd_res =
            std::find_if(old_confs.loggers.begin(), old_confs.loggers.end(), [&](const LoggerConfigVal &lcv) -> bool {
              return lcv.name == new_conf.name;
            });
        // if the logger exists, update it.
        if (fd_res != old_confs.loggers.end()) {
          this->setLogger(config_logger(new_conf));
        }
        // if the logger doesn't exist, add it.
        this->addLogger(config_logger(new_conf));
      }

      //Check if the old configuration is included in the new configuration
      for (const auto &old_conf: old_confs.loggers) {
        auto fd_res =
            std::find_if(old_confs.loggers.begin(), old_confs.loggers.end(), [&](const LoggerConfigVal &lcv) -> bool {
              return lcv.name == old_conf.name;
            });
        if (fd_res == new_confs.loggers.end()) {
          this->delLogger(fd_res->name);
        }
      }
    });
  }
  auto &conf = s_log_configs->getValue();
  m_defLevel = conf.def_level;
  m_defFormatter = std::make_shared<LogFormatter>(conf.def_pattern);
  for (const auto &lcv: conf.loggers) {
    this->addLogger(config_logger(lcv));
  }
  m_rootLogger = getLogger("root");
}

LogEvent::Level LoggerManager::getDefLevel() const { return m_defLevel; }
const LogFormatter::ptr &LoggerManager::getDefFormatter() const { return m_defFormatter; }
const Logger::ptr &LoggerManager::getRootLogger() const { return m_rootLogger; }

Logger::ptr LoggerManager::getLogger(const std::string &name)
{
  auto find_rt = m_loggers.find(name);
  if (find_rt == m_loggers.end()) {
    return nullptr;
  }
  return find_rt->second;
}

Logger::ptr LoggerManager::setLogger(const std::string &name, LogEvent::Level logger_level, LogFormatter::ptr formatter, const std::vector<LogAppender::ptr> &appenders)
{
  auto logger = getLogger(name);
  if (!logger)
    return logger;
  if (logger_level != LogEvent::Level::LL_UNKNOWN)
    logger->setLevel(logger_level);
  if (formatter)
    logger->setFormatter(std::move(formatter));
  for (const auto &appender: appenders)
    logger->addAppender(appender);
  return logger;
}

bool LoggerManager::setLogger(const Logger::ptr &logger)
{
  if (logger == nullptr || logger->getName().empty()) {
    return false;
  }
  auto fd_res = m_loggers.find(logger->getName());
  if (fd_res == m_loggers.end()) {
    return false;
  }
  fd_res->second = logger;
  return true;
}

Logger::ptr LoggerManager::addLogger(const std::string &name,
                                     LogEvent::Level logger_level,
                                     LogFormatter::ptr formatter,
                                     const std::vector<LogAppender::ptr> &appenders)
{
  auto res = getLogger(name);
  if (res) {
    return res;
  }

  Logger::ptr logger{new Logger(name, logger_level)};
  if (formatter)
    logger->setFormatter(std::move(formatter));

  for (const auto &appender: appenders)
    logger->addAppender(appender);

  return m_loggers.emplace(logger->getName(), logger).first->second;
}

bool LoggerManager::addLogger(const Logger::ptr &logger)
{
  auto res = getLogger(logger->getName());
  if (res) {
    return false;
  }
  m_loggers.emplace(logger->getName(), logger);
  return true;
}

bool LoggerManager::delLogger(const std::string &name)
{
  auto findRt = m_loggers.find(name);
  if (findRt != m_loggers.end()) {
    return false;
  }
  m_loggers.erase(findRt);
  return true;
}

Logger::ptr LoggerManager::config_logger(const LoggerConfigVal &conf)
{
  if (conf.name.empty()) {
    return nullptr;
  }
  auto level = conf.level == LogEvent::Level::LL_UNKNOWN ? m_defLevel : conf.level;

  Logger::ptr ret = std::make_shared<Logger>(conf.name, level);
  auto formatter = conf.pattern.empty()
                       ? m_defFormatter
                       : std::make_shared<LogFormatter>(conf.pattern);
  ret->setFormatter(formatter);
  for (const auto &app_conf: conf.appenders) {
    auto app = config_appender(level, formatter, app_conf);
    if (app == nullptr) { continue; }
    ret->addAppender(app);
  }
  return ret;
}

LogAppender::ptr LoggerManager::config_appender(LogEvent::Level logger_level, LogFormatter::ptr logger_formatter, const AppenderConfigVal &conf)
{
  if (conf.type == LogAppender::APD_UNDEFINE) {
    return nullptr;
  }
  auto formatter = conf.pattern.empty() ? std::move(logger_formatter) : std::make_shared<LogFormatter>(conf.pattern);
  LogEvent::Level level = conf.level == LogEvent::Level::LL_UNKNOWN ? logger_level : conf.level;
  LogAppender::ptr ret = nullptr;
  if (conf.type == LogAppender::APD_CONSOLE) {
    ret = std::make_shared<ConsoleAppender>(level);
    ret->setFormatter(formatter);
  } else if (conf.type == LogAppender::APD_FILE) {
    ret = std::make_shared<FileAppender>(level, conf.file);
    ret->setFormatter(formatter);
  }
  return ret;
}

//=====================================================================================================================

}// namespace mylib
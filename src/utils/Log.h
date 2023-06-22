//
// Created by HanHaocheng on 2023/06/06.
//

#ifndef THINK_LOG_H
#define THINK_LOG_H

#include <cassert>
#include <string>
#include <utility>

namespace thinksystem::utils
{
//TODO 日志系统未完成
enum class LogLevel
{
  UNKNOWN,
  INFO,
  WARRING,
  ERROR
};
struct LogInfo
{
  using level_type = LogLevel;
  using info_type  = std::string;

  level_type level;
  info_type info;

  LogInfo(level_type lv, info_type info) : level(lv), info(std::move(info)) {}
};

void Assert(bool as, LogInfo&& info)
{
  if (as) { exit(0); }
}

}// namespace thinksystem::utils

#endif//THINK_LOG_H

//
// Created by HanHaocheng on 2023/11/7.
//

#include "timer.h"
#include <ctime>
#include "timer.h"

namespace mylib
{
uint64_t Timer::GetCurrentTime()
{
  time_t current_time;
  time(&current_time);
  return current_time;
}
}// namespace myai
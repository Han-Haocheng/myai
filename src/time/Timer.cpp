//
// Created by HanHaocheng on 2023/11/7.
//

#include "Timer.h"
#include <ctime>

namespace myai
{
uint64_t Timer::GetCurrentTime()
{
  time_t current_time;
  time(&current_time);
  return current_time;
}
}// namespace myai
//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_AI_TIMER_H
#define MY_AI_TIMER_H

#include <cstdint>
namespace myai
{

//计时器
class Timer
{
public:
  //获取当前时间
  static uint64_t GetCurrentTime();
};

}// namespace myai

#endif//MY_AI_TIMER_H

//
// Created by HanHaocheng on 2023/11/7.
//

#ifndef MY_LIB_TIMER_H
#define MY_LIB_TIMER_H

#include <cstdint>
namespace mylib
{

//计时器
class Timer
{
public:
  //获取当前时间
  static uint64_t GetCurrentTime();
};

}// namespace myai

#endif//MY_LIB_TIMER_H

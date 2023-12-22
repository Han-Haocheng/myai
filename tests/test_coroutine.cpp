//
// Created by HanHaocheng on 2023/12/11.
//

#include "../src/log/LoggerManager.h"
#include "../src/thread/Coroutine.h"
std::function<void()> func1 = []() {
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "func1 start";

  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "Yield 1";
  myai::Coroutine::Yield();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "func1 end";
};

std::function<void()> func2 = [] {
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "func2 start";
  myai::Coroutine::ptr test_co = std::make_shared<myai::Coroutine>(func1);
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 0";
  test_co->resume();
  //  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 1";
  //  test_co->resume();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "func2 end";
};

void test2()
{
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << ">>>>>>>>>>>>>>>>>>>start";
  myai::Coroutine::ptr test_co = std::make_shared<myai::Coroutine>(func2);
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume";
  test_co->resume();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << ">>>>>>>>>>>>>>>>>>>end";
}

void test_base()
{
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "begin";
  myai::Coroutine::ptr test_co = std::make_shared<myai::Coroutine>(func1);
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 1";
  test_co->resume();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 2";
  test_co->resume();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "end";
}

int main(int argc, char **argv)
{
  test2();
  return 0;
}
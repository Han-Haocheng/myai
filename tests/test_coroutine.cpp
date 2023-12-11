//
// Created by HanHaocheng on 2023/12/11.
//

#include "../src/log/LoggerManager.h"
#include "../src/thread/Coroutine.h"
void test_base()
{
  myai::Coroutine::ptr test_co = std::make_shared<myai::Coroutine>([]() {
    myai::Coroutine::GetThis()->Yield();
    MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "Yield 1";
    myai::Coroutine::GetThis()->Yield();
    MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "Yield 2";
  });
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 1";
  test_co->resume();
  MYAI_LOG_DEBUG(MYAI_LOGGER_ROOT) << "resume 2";
  test_co->resume();
  //test_co->restart();
}

int main(int argc, char **argv)
{
  test_base();
  return 0;
}
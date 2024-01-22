//
// Created by HanHaocheng on 2023/12/28.
//

#define BOOST_TEST_MODULE MyLibTest
#include "log.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(suite_mylib_asyn)

std::function<void()> func1 = []() {
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "func1 start";

  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "Yield 1";
  mylib::Coroutine::Yield();
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "func1 end";
};

std::function<void()> func2 = [] {
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "func2 start";
  ::mylib::Coroutine::ptr test_co = std::make_shared<mylib::Coroutine>(func1);
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "resume 0";
  test_co->resume();
  //  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "resume 1";
  //  test_co->resume();
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "func2 end";
};

BOOST_AUTO_TEST_CASE(test_asyn_coroutine2)
{
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << ">>>>>>>>>>>>>>>>>>>start";
  mylib::Coroutine::ptr test_co = std::make_shared<mylib::Coroutine>(func2);
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "resume";
  test_co->resume();
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << ">>>>>>>>>>>>>>>>>>>end";
}

BOOST_AUTO_TEST_CASE(test_asyn_coroutine)
{
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "begin";
  mylib::Coroutine::ptr test_co = std::make_shared<mylib::Coroutine>(func1);
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "resume 1";
  test_co->resume();
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "resume 2";
  test_co->resume();
  MYLIB_LOG_DEBUG(MYLIB_LOGGER_ROOT) << "end";
  //BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
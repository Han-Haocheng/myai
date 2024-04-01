//
// Created by HanHaocheng on 2023/12/28.
//

#define BOOST_TEST_MODULE MyLibTest
#include <boost/test/unit_test.hpp>

#include "../mylib/log/log.h"

BOOST_AUTO_TEST_SUITE(suite_mylib_log)

BOOST_AUTO_TEST_CASE(test_log_base) {
  auto logger = MYLIB_LOGGER_NAME("test");
  logger->addAppender(std::make_shared<mylib::log::ConsoleAppender>(mylib::log::LogEvent::LL_INFO,
                                                                    std::make_shared<mylib::log::LogFormatter>("%d [%t] %p - %m%n")));
  MYLIB_LOG_INFO(logger) << "1234567890";
  //BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
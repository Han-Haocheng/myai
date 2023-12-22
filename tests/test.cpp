//
// Created by HanHaocheng on 2023/12/21.
//

#define BOOST_TEST_MODULE LogTests
#include <boost/test/unit_test.hpp>

#include "../include/log.h"
BOOST_AUTO_TEST_SUITE(suite_log)

BOOST_AUTO_TEST_CASE(test_fromString)
{
  for (int i = 0; i < 100000; ++i) {
    auto res = mylib::LogAppender::FromString("2");
  }
}

BOOST_AUTO_TEST_SUITE_END()
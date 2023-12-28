//
// Created by HanHaocheng on 2023/12/21.
//

#define BOOST_TEST_MODULE BoostTestTests
#include <boost/test/unit_test.hpp>

//#include "../include/log.h"
BOOST_AUTO_TEST_SUITE(suite_boost_test)

BOOST_AUTO_TEST_CASE(test_fromString)
{
  std::vector<int> vec = {1, 2, 3, 4, 5};

  for (auto iterator = vec.begin(); iterator != vec.end(); ++iterator) {
    static std::map<int, std::function<int()>> s_map = {
        {1, [&]() { return *iterator; }},
        {2, [&]() { return *iterator; }},
        {3, [&]() { return *iterator; }},
        {4, [&]() { return *iterator; }},
        {5, [&]() { return *iterator; }},
    };
    BOOST_TEST(s_map[*iterator]() == *iterator);
  }

}

BOOST_AUTO_TEST_SUITE_END()
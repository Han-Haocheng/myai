//
// Created by HanHaocheng on 2023/12/31.
//

#define BOOST_TEST_MODULE MyAiTest
#include "../node.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(suite_myai_node)

BOOST_AUTO_TEST_CASE(test_base)
{
  myai::NodeControl::ptr nodeControl = myai::NodeControl::getInstance();
  nodeControl->run();
}

BOOST_AUTO_TEST_SUITE_END()
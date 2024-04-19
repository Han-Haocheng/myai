//
// Created by HanHaocheng on 2024/4/12.
//

#include "../../mylib/log/logger_manager.h"
#include "../test.h"
#include <boost/test/unit_test.hpp>
#include <memory>
using String = std::string;

BOOST_AUTO_TEST_SUITE(log_suite_test)


BOOST_AUTO_TEST_CASE(test_log_base) {
  auto logger = MYLIB_LOG_NAME("test");
  MYLIB_LOG_DEBUG(logger) << "123123123123"
                          << "123123";
}

String m_pattern = "%z{%Y-%m-%d %H:%M:%S.%f}%a{}%b%c%d%e%fadfadfasddfasdfasdfasdfasdfasfdadf";

BOOST_AUTO_TEST_CASE(test_log_parse_pattern) {
  struct TmpFormat {
    String pattern;
    String extend;
    TmpFormat() = default;
    TmpFormat(String pattern, String extend) : pattern(std::move(pattern)), extend(std::move(extend)) {}
    [[nodiscard]] bool empty() const { return pattern.empty() && extend.empty(); };
  };

  String pattern = m_pattern;
  std::vector<TmpFormat> tmp_format_vec;
  TmpFormat tmp_format;

  for (size_t pos = 0; pos < m_pattern.size(); ++pos) {
    if (pattern[pos] != '%') {
      tmp_format.extend += pattern[pos];
      continue;
    }

    //检测复合格式化要求
    if (pos + 1 == m_pattern.size()) {
      tmp_format.extend += "%";
      continue;
    }
    if (!tmp_format.empty()) {
      tmp_format_vec.emplace_back(tmp_format);
      tmp_format = {};
    }

    //进入格式化模式
    tmp_format_vec.emplace_back((char[2]){pattern[pos++], pattern[pos]}, String{});

    // 检测是否符合扩展要求
    if (pattern[pos + 1] != '{') {
      //推出格式化模式
      continue;
    }

    auto &back_tf = tmp_format_vec.back();
    auto ex_end = pattern.find_first_of('}', pos + 2);
    if (ex_end == std::string::npos) {
      // 如果是扩展未闭合，则进入普通模式
      //pos = end_pos;
      continue;
    }
    back_tf.extend = pattern.substr(pos + 2, ex_end - pos - 2);
    pos = ex_end;
  }
  if (!tmp_format.empty()) {
    tmp_format_vec.emplace_back(tmp_format);
  }

  for (const auto &item: tmp_format_vec) {
    std::cout << "pattern:" << item.pattern << "\textend:" << item.extend << std::endl;
  }
}

BOOST_AUTO_TEST_SUITE_END()
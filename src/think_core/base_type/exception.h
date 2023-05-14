//
// Created by HanHaocheng on 2023/05/08.
//

#ifndef THINKSYSTEM_EXCEPTION_H
#define THINKSYSTEM_EXCEPTION_H
#include <exception>
#include <utility>

#include "../CoreInclude.h"

class think_error : public std::exception
{
  std::string m_what_;
  std::error_code m_ec_;

public:
  think_error(std::string what, std::error_code ec) : m_what_(std::move(what)), m_ec_(ec) {}
  [[nodiscard]] const char *what() const noexcept final { return m_what_.c_str(); }
  std::error_code error_code() { return m_ec_; }
};
#endif//THINKSYSTEM_EXCEPTION_H

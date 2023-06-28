//
// Created by HanHaocheng on 2023/06/09.
//

#ifndef THINK_FILESYSTEM_H
#define THINK_FILESYSTEM_H
#if __cplusplus > 201703L || _HAS_CXX17 == 1

#include <filesystem>
namespace fs = ::std::filesystem;

#else

#include <io.h>

#include <list>
#include <string>

namespace thinksystem::utils::filesystem
{

class path
{
  std::string m_path_;
  std::list<std::string> m_ls_;

public:
  path(std::string _p) : m_path_(std::move(_p)) {}
  path(const path &_p) = default;
  path(path &&_p)      = default;
  [[nodiscard]] std::string str() const { return m_path_; }
  [[nodiscard]] const char *c_str() const { return m_path_.c_str(); }

public:
};

bool exist(const path &_p) { return access(_p.c_str(), F_OK) == 0; }

}// namespace thinksystem::utils::filesystem

namespace fs = thinksystem::utils::filesystem;
#endif

#endif//THINK_FILESYSTEM_H

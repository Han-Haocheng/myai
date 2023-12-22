//
// Created by HanHaocheng on 2023/12/15.
//

#ifndef THINK_DRIVER_H
#define THINK_DRIVER_H
#include <memory>
#include <mysql/mysql.h>

namespace myai
{

class Connection;

class Driver
{

public:
  using ptr = std::shared_ptr<Driver>;
  static Driver::ptr GetInstance();

  std::shared_ptr<Connection> getConection();

private:
  Driver() = default;
  void init();

private:
};

}// namespace myai

#endif//THINK_DRIVER_H

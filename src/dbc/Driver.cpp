//
// Created by HanHaocheng on 2023/12/15.
//

#include "Driver.h"

namespace myai
{
Driver::ptr Driver::GetInstance()
{
  mysql_library_init();
  return myai::Driver::ptr();
}
std::shared_ptr<Connection> Driver::getConection()
{
  return std::shared_ptr<Connection>();
}

void Driver::init()
{
}

}// namespace myai
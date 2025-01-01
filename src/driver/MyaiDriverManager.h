#pragma once

#include "../driver/MyaiDriver.h"

MYAI_BEGIN

class MyaiDriverManager {
public:
  using ptr = std::shared_ptr<MyaiDriverManager>;

  bool loadConfig();
  bool saveConfig();

  MyaiDriver::ptr addDriver(IdAllocator idalloc, MyaiDriver::Type ty);

  void cellect(LinkStream::ptr input);
  void control(LinkStream::ptr output);

private:
  std::vector<MyaiDriver::ptr> m_drivers;
};

MYAI_END
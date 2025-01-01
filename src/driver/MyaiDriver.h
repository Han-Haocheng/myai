#pragma once

#include "../dao/LinkStream.h"
#include <functional>

MYAI_BEGIN

/**
 * @brief 驱动
 */
class MyaiDriver {
public:
  using ptr = std::shared_ptr<MyaiDriver>;
  using DriverFactory = std::function<MyaiDriver::ptr(nodeid_t)>;
  enum Type
  {
    DT_EMOTION,
    DT_STRING,
    DT_AUDIO,
    DT_SCREEN_VIDEO,
    DT_CAMERA_VIDEO,
  };

  MyaiDriver(Type type, nodeid_t begin, size_t id_size);
  virtual ~MyaiDriver() = default;
  virtual void collect(LinkStream::ptr input) = 0;
  virtual void control(LinkStream::ptr output) = 0;

  static std::map<MyaiDriver::Type, DriverFactory> S_FACTORIES;

protected:
  Type m_type;
  nodeid_t m_begin;
  size_t m_id_size;
};

MYAI_END

MYAI_BEGIN

//size = 4H+4H
class EmotionDriver : public MyaiDriver {
};

MYAI_END

MYAI_BEGIN

//size = 1 0000H
class StringDriver : public MyaiDriver {
};

MYAI_END

MYAI_BEGIN

//size = 800H+800H
class AudioDriver : public MyaiDriver {
public:
private:
};

MYAI_END

MYAI_BEGIN

//size = 40 0000H+4H
class ScreenVideoDriver : public MyaiDriver {
};

//size = 40 0000H+8H
class CameraVideoDriver : public MyaiDriver {
};

MYAI_END

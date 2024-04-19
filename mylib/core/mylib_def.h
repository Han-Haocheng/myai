#ifndef MYLIB_DEF_H_
#define MYLIB_DEF_H_
#include <sstream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

#define MYLIB_SPACE_NAME mylib
#define MYLIB_SPACE_BEGIN namespace MYLIB_SPACE_NAME {
#define MYLIB_SPACE_END }

MYLIB_SPACE_BEGIN

using int8 = signed char;
using int16 = signed short int;
using int32 = signed int;
using int64 = signed long long int;

using uint8 = unsigned char;
using uint16 = unsigned short int;
using uint32 = unsigned int;
using uint64 = unsigned long long int;

using float32 = float;
using float64 = double;
using CString = const char *;


using tid_t = uint32;// 线程id类型
using cid_t = uint;  //协程id类型
using size_t = uint64;
using weight_t = float32;

using String = std::string;
using SString = std::stringstream;

using time_t = ::time_t;//时间戳类型

MYLIB_SPACE_END

#endif// MYLIB_DEF_H_
#ifndef MY_LIB_TYPE_H_
#define MY_LIB_TYPE_H_

#include <string>
#include <sstream>

#define MYLIB_BEGIN namespace mylib {
#define MYLIB_END }

MYLIB_BEGIN

using int64 = long long;
using uint64 = unsigned long long;
using int32 = int;
using uint32 = unsigned int;
using int16 = short;
using uint16 = unsigned short;
using int8 = char;
using uint8 = unsigned char;

using double32 = float;
using double64 = double;
using boolean = bool;

using String = std::string;
using SString = std::stringstream;

MYLIB_END

#endif// MY_LIB_TYPE_H_

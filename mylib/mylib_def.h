#ifndef MYLIB_DEF_H_
#define MYLIB_DEF_H_
#include <sstream>
#include <string>


#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

#define MYLIB_SPACE_BEGIN namespace mylib {
#define MYLIB_SPACE_END }


MYLIB_SPACE_BEGIN

int test();

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

using boolean = bool;
using nid_t = uint64;
using tid_t = uint32;
using size_t = uint64;
using weight_t = float32;

using String = std::string;
using SString = std::stringstream;

MYLIB_SPACE_END

#endif// MYLIB_DEF_H_
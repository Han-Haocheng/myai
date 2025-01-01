#ifndef MYAI_DEF_H_
#define MYAI_DEF_H_

#define MYAI_SPACE_BEGIN namespace myai {
#define MYAI_SPACE_END }
#include <sstream>
#include <string>

MYAI_SPACE_BEGIN

using int8     = char;
using int16    = short;
using int32    = int;
using int64    = long long;

using uint8    = unsigned char;
using uint16   = unsigned short;
using uint32   = unsigned int;
using uint64   = unsigned long long;

using float32  = float;
using float64  = double;

using boolean  = bool;
using nid_t    = uint64;
using tid_t    = uint32;
using size_t   = uint64;
using weight_t = float32;

using String   = std::string;
using SString  = std::stringstream;

MYAI_SPACE_END

#endif//MYAI_DEF_H_

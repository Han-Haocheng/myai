#ifndef MYAI_TYPE_H_
#define MYAI_TYPE_H_

#include <mylib/base/mylib_def.h>

#define MYAI_SPACE myai
#define MYAI_BEGIN             \
  using namespace MYLIB_SPACE; \
  namespace MYAI_SPACE {
#define MYAI_END }

MYAI_BEGIN

using nodeid_t                                   = uint64;
using weight_t                                   = float;
constexpr const static nodeid_t UNDEFINED_NODE_ID = -1;

MYAI_END

#endif// !MYAI_TYPE_H_

#pragma once
#include <mylib_core/core.h>


#define MYAI_BEGIN   \
	namespace myai { \
	using namespace MYLIB_SPACE;
#define MYAI_END }

MYAI_BEGIN

using nodeid_t = uint32;
using weight_t = float;

MYAI_END
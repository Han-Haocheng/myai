#pragma once

#include <mylib/core/core.h>


#define MYAI_BEGIN   \
	namespace myai { \
	using namespace MYLIB_SPACE;
#define MYAI_END }

MYAI_BEGIN

using nodeid_t = uint32;
using weight_t = float;
using byte_t   = char;

class ISerialize {
public:
	virtual ~ISerialize()							= default;
	virtual void serialize(std::ostream &out) const = 0;
	virtual void deserialize(std::istream &in)		= 0;
};

MYAI_END
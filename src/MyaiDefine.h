#ifndef MYAI_DEFINE_H_
#define MYAI_DEFINE_H_


#include <mylib/core/core.h>
#include <mylib/log/log.h>


#define MYAI_SPACE myai
#define MYAI_BEGIN namespace MYAI_SPACE { 	using namespace MYLIB_SPACE;
#define MYAI_END }

MYAI_BEGIN

using nodeid_t = uint32;
typedef nodeid_t noid_t, edgeid_t;
using weight_t = float;
using byte_t = char;

constexpr static const nodeid_t NULL_ID		= 0ULL;
constexpr static const weight_t NULL_WEIGHT = 0.0;

class ISerialize {
public:
	virtual ~ISerialize() = default;
	virtual void serialize(std::ostream &out) const = 0;
	virtual void deserialize(std::istream &in) = 0;
};

MYAI_END

#endif// !MYAI_DEFINE_H_

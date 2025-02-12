#ifndef MYAI_DEFINE_H_
#define MYAI_DEFINE_H_


#include <mylib/core/core.h>

<<<<<<< HEAD
<<<<<<< HEAD
#define MYAI_SPACE myai
#define MYAI_BEGIN         \
	namespace MYAI_SPACE { \
=======

#define MYAI_BEGIN   \
	namespace myai { \
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
#define MYAI_SPACE myai
#define MYAI_BEGIN   \
	namespace MYAI_SPACE { \
>>>>>>> cf44269 (2025年2月23日 16:12:23)
	using namespace MYLIB_SPACE;
#define MYAI_END }

MYAI_BEGIN

using nodeid_t = uint32;
<<<<<<< HEAD
typedef nodeid_t noid_t, edgeid_t;
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
using weight_t = float;
using byte_t   = char;

class ISerialize {
public:
	virtual ~ISerialize()							= default;
	virtual void serialize(std::ostream &out) const = 0;
	virtual void deserialize(std::istream &in)		= 0;
};

<<<<<<< HEAD
=======



>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
MYAI_END

#endif// !MYAI_DEFINE_H_

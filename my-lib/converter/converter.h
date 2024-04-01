#pragma once
#include "../mylib_def.h"


MYLIB_SPACE_BEGIN

template<typename From, typename To>
To cast(const From &from) {
  return (To) from;
}


MYLIB_SPACE_END

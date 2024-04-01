#pragma once
#include "converter.h"
#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>

MYLIB_SPACE_BEGIN

template<typename Ty>
class YmalConverter {
public:
  Ty from(const String &yaml) {
    return boost::lexical_cast<Ty>(yaml);
  }

  String to(const Ty &ty) {
    return boost::lexical_cast<String>(yaml);
  }
};

template<typename Ty>
class YmalConverter<std::vector<Ty>> {
public:
  std::vector<Ty> from(const String &yaml) {
    return boost::lexical_cast<Ty>(yaml);
  }

  String to(const std::vector<Ty> &ty) {
    for (auto &ele: ty) {
      
    }
    return boost::lexical_cast<String>(yaml);
  }
};

MYLIB_SPACE_END

#ifndef MYLIB_YMAL_CONVERTER_H
#define MYLIB_YMAL_CONVERTER_H

#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "converter.h"

MYLIB_SPACE_BEGIN

template<typename Ty>
class YamlConverter : private StringConverter<Ty> {
public:
  const Ty& fromString(const String &yaml) {
    return StringConverter<Ty>::fromString(yaml);
  }

  const String &toString(const Ty &ty) {
    return StringConverter<Ty>::toString(ty);
  }
};

template<typename Ty>
class YamlConverter<std::vector<Ty>> {
public:
  using CastTy = std::vector<Ty>;
  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy ty;
    for (YAML::detail::iterator_value iter: node) {
      ty.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return std::move(ty);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }

    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::list<Ty>> {
public:
  using CastTy = std::list<Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (YAML::detail::iterator_value iter: node) {
      castVal.emplace_back(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::forward_list<Ty>> {
public:
  using CastTy = std::forward_list<Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (YAML::detail::iterator_value iter: node) {
      castVal.emplace_front(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::set<Ty>> {
public:
  using CastTy = std::set<Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (YAML::detail::iterator_value iter: node) {
      castVal.emplace(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_set<Ty>> {
public:
  using CastTy = std::unordered_set<Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (YAML::detail::iterator_value iter: node) {
      castVal.emplace(YamlConverter<Ty>{}.fromString(YAML::Dump(iter)));
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::map<String, Ty>> {
public:
  using CastTy = std::map<String, Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (auto iter: node) {
      std::pair<String, Ty> a{iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first),
                              YamlConverter<Ty>{}.fromString(YAML::Dump(iter.second))};

      castVal.emplace(a);
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

template<typename Ty>
class YamlConverter<std::unordered_map<String, Ty>> {
public:
  using CastTy = std::unordered_map<String, Ty>;

  CastTy &&fromString(const String &yaml) {
    YAML::Node node = YAML::Load(yaml);
    CastTy castVal;
    for (auto iter: node) {
      std::pair<String, Ty> a{iter.first.IsScalar() ? iter.first.Scalar() : YAML::Dump(iter.first),
                              YamlConverter<Ty>{}.fromString(YAML::Dump(iter.second))};

      castVal.emplace(a);
    }

    return std::move(castVal);
  }

  String toString(const CastTy &ty) {
    YAML::Node node;
    for (auto &ele: ty) {
      node.push_back(YamlConverter<Ty>{}.toString(ele));
    }
    return YAML::Dump(node);
  }
};

MYLIB_SPACE_END

#endif

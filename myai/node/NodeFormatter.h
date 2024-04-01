//
// Created by HanHaocheng on 2024/2/19.
//

#ifndef MY_AI_NODEFORMATTER_H
#define MY_AI_NODEFORMATTER_H
#include "../../mylib/format/format.h"
#include "Node.h"

MYLIB_FORMAT_BEGIN
template<>
class Formatter<MYAI::Node::Type> {
public:
  using Type = MYAI::Node::Type;

  std::string toString(Type val) {
    switch (val) {
#define XX(ty)        \
  case Type::NT_##ty: \
    return #ty
      XX(UNKNOWN);
      XX(MEMORY);
      XX(EMOTION);
      XX(RECORD);
    }
#undef XX
    return "UNKNOWN";
  }

  Type fromString(const std::string &str) {
    static std::unordered_map<std::string, Type> s_nodeTy_str_map ={
      {"0", Type::NT_EMOTION},
          {"emotion", Type::NT_EMOTION},
          {"Emotion", Type::NT_EMOTION},
          {"EMOTION", Type::NT_EMOTION},
          {"NT_EMOTION", Type::NT_EMOTION},

          {"1", Type::NT_MEMORY},
          {"memory", Type::NT_MEMORY},
          {"Memory", Type::NT_MEMORY},
          {"MEMORY", Type::NT_MEMORY},
          {"NT_MEMORY", Type::NT_MEMORY},
    };
    auto res = s_nodeTy_str_map.find(str);
    return res != s_nodeTy_str_map.end() ? res->second : Type::NT_UNKNOWN;
  }
};
MYLIB_FORMAT_END
#endif//MY_AI_NODEFORMATTER_H

//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_0_THINK_ENUM_H
#define THINKSYSTEM_0_THINK_ENUM_H
#include "include.h"

namespace think {

typedef enum class ENodeType {
  NULL_TYPE    = 0,
  DYNAMIC_TYPE = 1,
  STATIC_TYPE  = 2,
  CONST_TYPE   = 4,
} NODETYPE;

typedef enum class EConstType : unsigned char {
  IN_MEMORY,
  OUT_MEMORY,
  IN_VISION,
  OUT_VISION,
} CONSTTYPE;

const static std::unordered_map<CONSTTYPE, std::string> MAP_CONST_TYPE_TO_STRING{
        //        {NodeType::NULL_TYPE, "null_type"},
        //              {NodeType::STATIC_TYPE, "static"},
        //        {NodeType::DYNAMIC_TYPE, "dynamic"},
        {CONSTTYPE::IN_VISION, "const_in_vision"},
        {CONSTTYPE::OUT_VISION, "const_out_vision"},
};

}// namespace think

#endif//THINKSYSTEM_0_THINK_ENUM_H

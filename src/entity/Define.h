//
// Created by HanHaocheng on 2023/06/10.
//

#ifndef THINK_DEFINE_H
#define THINK_DEFINE_H
#include <sstream>

#include "../utils/BoundedValue.h"
#include "../utils/Funcation.h"

namespace thinksystem
{

#define interface class

using id_t     = unsigned long long int;
using link_t   = unsigned int;
using size_t   = unsigned long long;
using weight_t = utils::BoundedValue<double>;
using byte_t   = char;
#define STR_NODE_LIB_PATH ".\\nodelib\\"
#define STR_NODE_FILE_NAME "node.dat"

#define VAL_DEFINE_ID_TYPE 0x0
#define VAL_MIN_ID_TYPE 0x1
#define VAL_MAX_ID_TYPE 0xffffffffffff

#define VAL_DEFINE_LINK_TYPE 0x0
#define VAL_MIN_LINK_TYPE 0x1
#define VAL_MAX_LINK_TYPE 0xffffffff

namespace utils
{

template<>
std::string to_path<id_t>(id_t id)
{
  std::stringstream ss(STR_NODE_LIB_PATH);
  for (size_t i = sizeof(unsigned long long); i > 0; --i) ss << static_cast<int>((id >> (8 * (i - 1))) & 0xFF) << "\\";

  return ss.str();
}

}// namespace utils

}// namespace thinksystem
#endif//THINK_DEFINE_H

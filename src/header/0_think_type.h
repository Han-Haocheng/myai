//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_0_THINK_TYPE_H
#define THINKSYSTEM_0_THINK_TYPE_H
#include <utility>

#include "0_think_enum.h"
#include "include.h"

#define GetValInRange(min_val, max_val, val) (std::max(min_val, std::min(max_val, val)))


namespace think {
#pragma pack(push, 2)

static const constexpr size_t NUM_NODE_ID_MAX   = 0x1000000000000U;


constexpr static const char *STR_NODE_LIB_PATH  = "./nodelib/";
constexpr static const char *STR_NODE_FILE_NAME = "node.dat";

using NODE_ID                                   = size_t;
using C_NODE_ID                                 = const NODE_ID;
using UP_NODE_ID                                = std::unique_ptr<NODE_ID>;
using UPC_NODE_ID                               = std::unique_ptr<C_NODE_ID>;


[[nodiscard]] std::string GetNodeParentPath(NODE_ID id) {
  std::string res = STR_NODE_LIB_PATH;
  for (size_t i = 0U; i < sizeof(NODE_ID); ++i) {
    res += std::to_string(id & 0xff) + "/";
    id >>= 8;
  }
  return res;
}

[[nodiscard]] std::string GetNodePath(NODE_ID id) {
  return GetNodeParentPath(id) + STR_NODE_FILE_NAME;
}


#define NUM_LINK_VAL_MAX 0xffff
#define NUM_LINK_VAL_MIN 0x0
using active_t = size_t;

using link_t   = uint16_t;
extern class CNodeInfo;
using node_t = std::pair<NODE_ID, CNodeInfo>;
struct slink_t {
  node_t *pNode         = nullptr;
  link_t uiValLink               = NUM_LINK_VAL_MIN;
  slink_t()                = default;
  slink_t(slink_t &&)      = default;
  slink_t(const slink_t &) = default;
  slink_t(std::pair<NODE_ID, CNodeInfo> *id, link_t val) : pNode(id), uiValLink(val) {}
  slink_t &operator=(slink_t &&)      = default;
  slink_t &operator=(const slink_t &) = default;
};
// 软性（浮点型）链接会导致链接值的局部性，从而破坏思维链接一致性原则，因此链接必须是硬性（整型）的。

// 软性链接列表

using ALL_LINK_VAL = size_t;
using WEIGHT       = double;


#pragma pack(pop)

}// namespace think

#endif//THINKSYSTEM_0_THINK_TYPE_H

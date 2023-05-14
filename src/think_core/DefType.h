//
// Created by HanHaocheng on 2023/04/27.
//

#ifndef THINKSYSTEM_THINKTYPE_H
#define THINKSYSTEM_THINKTYPE_H

#include "CoreInclude.h"

#define GetValInRange(min_val, max_val, val) (std::max(min_val, std::min(max_val, val)))

namespace think
{

constexpr static const char *STR_NODE_LIB_PATH  = "./nodelib/";
constexpr static const char *STR_NODE_FILE_NAME = "id.dat";

using node_id                                   = size_t;
using C_NODE_ID                                 = const node_id;
using UP_NODE_ID                                = std::unique_ptr<node_id>;
using UPC_NODE_ID                               = std::unique_ptr<C_NODE_ID>;

static const constexpr node_id NUM_NODE_ID_MAX  = 0x1000000000000U;
static const constexpr node_id NUM_NODE_ID_MIN  = 0x0;

[[nodiscard]] std::string GetNodeParentPath(node_id id)
{
  std::string res = STR_NODE_LIB_PATH;
  for (size_t i = 0U; i < sizeof(node_id); ++i)
  {
    res += std::to_string(id & 0xff) + "/";
    id >>= 8;
  }
  return res;
}

[[nodiscard]] std::string GetNodePath(node_id id) { return GetNodeParentPath(id) + STR_NODE_FILE_NAME; }

using active_t                                   = size_t;

using link_val                                   = uint16_t;

static const constexpr link_val NUM_LINK_VAL_MAX = 0xffff;
static const constexpr link_val NUM_LINK_VAL_MIN = 0x0;

template<typename OtherType>
class WEIGHT
{
public:
  using weight_type = double;
  using other_type  = OtherType;
  static_assert(std::is_arithmetic_v<OtherType>, "权重类型只能和基本数据类型进行计算");

  constexpr WEIGHT() = default;
  constexpr WEIGHT(weight_type val) : m_val_(val) {}

  WEIGHT &operator=(WEIGHT &&) noexcept = default;
  WEIGHT &operator=(const WEIGHT &)     = default;

  WEIGHT &operator=(weight_type rhs)
  {
    m_val_ = ::std::min(::std::max(rhs, m_min_), m_max_);
    return *this;
  }

  constexpr WEIGHT(weight_type val, weight_type weightMax, weight_type weightMin)
  : m_val_(val),
    m_max_(weightMax),
    m_min_(weightMin)
  {
  }

  friend constexpr other_type operator+(const WEIGHT &lhs, const other_type &rhs)
  {
    return std::clamp(lhs.m_val_ + rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr other_type operator-(const WEIGHT &lhs, const other_type &rhs)
  {
    return std::clamp(lhs.m_val_ - rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr other_type operator*(const WEIGHT &lhs, const other_type &rhs)
  {
    return std::clamp(lhs.m_val_ * rhs, lhs.m_min_, lhs.m_max_);
  }
  friend constexpr other_type operator/(const WEIGHT &lhs, const other_type &rhs)
  {
    return std::clamp(lhs.m_val_ / rhs, lhs.m_min_, lhs.m_max_);
  }

  constexpr WEIGHT &operator+=(const other_type &rhs) { return *this = *this + rhs; }
  constexpr WEIGHT &operator-=(const other_type &rhs) { return *this = *this - rhs; }
  constexpr WEIGHT &operator*=(const other_type &rhs) { return *this = *this * rhs; }
  constexpr WEIGHT &operator/=(const other_type &rhs) { return *this = *this / rhs; }

  [[nodiscard]] constexpr weight_type val() const { return m_val_; }
  void setMax(weight_type mMax) { m_max_ = mMax; }
  void setMin(weight_type mMin) { m_min_ = mMin; }

private:
  weight_type m_val_ = 0.0;
  weight_type m_max_ = 1.0;
  weight_type m_min_ = 0.0;
};

using active_t = size_t;
using ActFunc  = void (*)(active_t);

enum class ENodeType
{
  NULL_TYPE,
  DYNAMIC_TYPE,
  STATIC_TYPE,
  CONST_TYPE,

  CONST_MEMORY,//思维的感知
  CONST_VIDEO, //图像的感知
  CONST_AUDIO, //声音的感知
  CONST_ACTION,//行为的感知
};

constexpr const static char *ARRAY_TYPE_TO_STRING[]{
        "null", "dynamic", "static", "const", "const_memory", "const_video", "const_audio", "const_action",
};

#pragma pack(push, 2)
struct link_t
{

  node_id id             = NUM_NODE_ID_MIN;
  link_val linkVal       = NUM_LINK_VAL_MIN;
  link_t()               = default;
  link_t(link_t &&)      = default;
  link_t(const link_t &) = default;
  link_t(node_id id, link_val linkVal) : id(id), linkVal(linkVal) {}

  link_t &operator=(link_t &&)      = default;
  link_t &operator=(const link_t &) = default;

  bool operator==(const link_t &rhs) const { return id == rhs.id; }
  bool operator!=(const link_t &rhs) const { return !(rhs == *this); }
  bool operator<(const link_t &rhs) const { return id < rhs.id; }
  bool operator>(const link_t &rhs) const { return rhs < *this; }
  bool operator<=(const link_t &rhs) const { return !(rhs < *this); }
  bool operator>=(const link_t &rhs) const { return !(*this < rhs); }
};

struct LinkHash
{
  size_t operator()(const link_t &rhs) const { return std::hash<node_id>{}(rhs.id); }
};

#pragma pack(pop)
}// namespace think

#endif//THINKSYSTEM_THINKTYPE_H

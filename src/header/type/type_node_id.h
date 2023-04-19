//
// Created by HanHaocheng on 2023/04/16.
//

#ifndef THINKSYSTEM_TYPE_NODE_ID_H
#define THINKSYSTEM_TYPE_NODE_ID_H


#include <string>
typedef unsigned char byte_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long long ulong_t, size_t;


constexpr const static byte_t COUNT_BYTE_BITS = 8;

template<size_t Bits>
constexpr static const size_t GET_SPECIFIC_NUM_BITS = (1 << Bits) - 1;

namespace think {

constexpr const static size_t COUNT_MAX_ID_SIZE = 0x1000000000000U;

//max = 17,5921,8604,4416
struct NodeID {
  using tab_type = ushort_t;
  using col_type = ushort_t;
  using row_type = ushort_t;

  using id_type  = size_t;

  tab_type tab_id;//表id  max:0xffff
  col_type col_id;//列id  max:0x0fff
  row_type row_id;//行id  max:0xffff


  constexpr static const byte_t COUNT_TAB_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;
  constexpr static const byte_t COUNT_COL_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;
  constexpr static const byte_t COUNT_ROW_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;

  NodeID() : tab_id(), col_id(), row_id() {}
  explicit NodeID(id_type id)
      : tab_id(static_cast<tab_type>(id >> COUNT_TAB_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)),
        col_id(static_cast<col_type>(id >> COUNT_COL_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)),
        row_id(static_cast<row_type>(id >> COUNT_ROW_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)) {}

  NodeID &operator=(id_type id) {
    reset(id);
    return *this;
  }

  bool operator<(const NodeID &rhs) const { return static_cast<id_type>(*this) < static_cast<id_type>(rhs); }
  bool operator>(const NodeID &rhs) const { return rhs < *this; }
  bool operator<=(const NodeID &rhs) const { return !(rhs < *this); }
  bool operator>=(const NodeID &rhs) const { return !(*this < rhs); }
  bool operator==(const NodeID &rhs) const { return static_cast<id_type>(*this) == static_cast<id_type>(rhs); }
  bool operator!=(const NodeID &rhs) const { return !(rhs == *this); }

  NodeID operator+(id_type num) const { return NodeID{static_cast<id_type>(*this) + num}; }
  NodeID operator-(id_type num) const { return NodeID{static_cast<id_type>(*this) - num}; }
  size_t operator-(const NodeID &other) const { return static_cast<id_type>(*this) - static_cast<id_type>(other); }

  NodeID &operator+=(id_type num) {
    this->reset(static_cast<id_type>(*this) + num);
    return *this;
  }
  NodeID &operator-=(id_type num) {
    this->reset(static_cast<id_type>(*this) - num);
    return *this;
  }
  NodeID &operator++() { return (*this) += 1; }
  NodeID &operator--() { return (*this) -= 1; }
  NodeID operator++(int) {
    NodeID tmp = *this;
    ++*this;
    return tmp;
  }
  NodeID operator--(int) {
    NodeID tmp = *this;
    --*this;
    return tmp;
  }

  inline explicit operator id_type() const {
    return (static_cast<id_type>(tab_id) << COUNT_TAB_OFFSET_BIT) |
           (static_cast<id_type>(col_id) << COUNT_COL_OFFSET_BIT) |
           (static_cast<id_type>(row_id) << COUNT_ROW_OFFSET_BIT);
  }

  void reset(id_type id) {
    this->tab_id = static_cast<tab_type>(id >> COUNT_TAB_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>);
    this->col_id = static_cast<col_type>(id >> COUNT_COL_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>);
    this->row_id = static_cast<row_type>(id >> COUNT_ROW_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>);
  }

  [[nodiscard]] std::string id_path() const {
    std::string res = "./data_lib/";
    auto tmp        = static_cast<id_type>(*this);
    for (size_t i = 0U; i < sizeof(*this); ++i) {
      res = std::to_string(tmp & 0xff) + "/";
      tmp >>= COUNT_BYTE_BITS;
    }
    return res;
  }
};//!struct NodeID

}// namespace think

#endif//THINKSYSTEM_TYPE_NODE_ID_H

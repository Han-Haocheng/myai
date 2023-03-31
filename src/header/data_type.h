//
// Created by HanHaocheng on 2023/03/29.
//

#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

#include <algorithm>
#include <string>
#include <vector>


namespace think {
#pragma pack(push, 2)

typedef unsigned char byte_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long long ulong_t, size_t;

constexpr const static byte_t COUNT_BYTE_BITS = 8;

template<size_t Bits>
constexpr static const size_t GET_SPECIFIC_NUM_BITS = (1 << Bits) - 1;

//max = 17,5921,8604,4416
typedef struct NodeIDType {
  using tab_type = ushort_t;
  using col_type = ushort_t;
  using row_type = ushort_t;

  using id_type  = size_t;

  tab_type tab_id;//表id  max:0xffff
  col_type col_id;//列id  max:0x0fff
  row_type row_id;//行id  max:0xffff

  constexpr static const char *CSTR_TAB_PREFIX       = "DATA_TAB_";
  constexpr static const char *CSTR_COL_PREFIX       = "COL_";

  constexpr static const byte_t COUNT_TAB_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;
  constexpr static const byte_t COUNT_COL_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;
  constexpr static const byte_t COUNT_ROW_OFFSET_BIT = (sizeof(col_id) + sizeof(row_id)) * COUNT_BYTE_BITS;

  NodeIDType() : tab_id(), col_id(), row_id() {}
  explicit NodeIDType(id_type id)
      : tab_id(static_cast<tab_type>(id >> COUNT_TAB_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)),
        col_id(static_cast<col_type>(id >> COUNT_COL_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)),
        row_id(static_cast<row_type>(id >> COUNT_ROW_OFFSET_BIT & GET_SPECIFIC_NUM_BITS<sizeof(tab_id) * COUNT_BYTE_BITS>)) {}

  NodeIDType &operator=(id_type id) {
    reset(id);
    return *this;
  }

  bool operator<(const NodeIDType &rhs) const { return static_cast<id_type>(*this) < static_cast<id_type>(rhs); }
  bool operator>(const NodeIDType &rhs) const { return rhs < *this; }
  bool operator<=(const NodeIDType &rhs) const { return !(rhs < *this); }
  bool operator>=(const NodeIDType &rhs) const { return !(*this < rhs); }
  bool operator==(const NodeIDType &rhs) const { return static_cast<id_type>(*this) == static_cast<id_type>(rhs); }
  bool operator!=(const NodeIDType &rhs) const { return !(rhs == *this); }

  NodeIDType operator+(id_type num) const { return NodeIDType{static_cast<id_type>(*this) + num}; }
  NodeIDType operator-(id_type num) const { return NodeIDType{static_cast<id_type>(*this) - num}; }
  size_t operator-(const NodeIDType &other) const { return static_cast<id_type>(*this) - static_cast<id_type>(other); }

  NodeIDType &operator+=(id_type num) {
    this->reset(static_cast<id_type>(*this) + num);
    return *this;
  }
  NodeIDType &operator-=(id_type num) {
    this->reset(static_cast<id_type>(*this) - num);
    return *this;
  }
  NodeIDType &operator++() { return (*this) += 1; }
  NodeIDType &operator--() { return (*this) -= 1; }
  NodeIDType operator++(int) {
    NodeIDType tmp = *this;
    ++*this;
    return tmp;
  }
  NodeIDType operator--(int) {
    NodeIDType tmp = *this;
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
} NodeID;//!struct NodeIDType


typedef enum class NodeTypeEnum {
  CONST_IN_VISION,
  CONST_OUT_VISION,

  STATIC,
  DYNAMIC,
} NodeType;

typedef struct ElementInformationType {
  using link_type = ushort_t;

  NodeID node_id;
  link_type link_val = 0;

} EleInfo;

typedef struct NodeInformationType {
  NodeType node_type   = NodeType::DYNAMIC;
/*
  size_t const_count   = 0;
  size_t static_count  = 0;
  size_t dynamic_count = 0;*/

  size_t const_value   = 0;
  size_t static_value  = 0;
  size_t dynamic_value = 0;
} NodeInfo;

using LinkList = std::vector<EleInfo>;

struct NodeLinks {
  LinkList const_links;
  LinkList static_links;
  LinkList dynamic_links;
  //  Node(const NodeID &m_id_, const NodeInfo &m_info_) : m_node_id_(m_id_), m_node_info_(m_info_) {}
};

/*class Node {
  friend class ThinkCore;

private:
  size_t m_tmp_link_val_ = 0;
  NodeInfo m_node_info;
  std::vector<EleInfo> m_links_;
  std::vector<EleInfo> m_links_buffer_;

public:
  [[nodiscard]] NodeID nodeID() const { return m_node_info.node_id; }

  void addLink(const EleInfo &newLink) {
    m_tmp_link_val_ += newLink.static_value;
    m_links_buffer_.push_back(newLink);
  }

  /// @brief 获取激活节点
  /// @param out 返回的激活节点
  /// @param actWeight 激活权重
  /// @return 激活节点总值
  size_t getActLinks (std::vector<EleInfo> &out, double actWeight) {
    out.reserve(m_node_info.static_count);
    for (const auto &item: m_links_) {
      out.push_back(EleInfo{item.node_id, static_cast<EleInfo::link_type>(item.static_value * actWeight)});
    }
    for (const auto &item: m_links_buffer_) {
      out.push_back(EleInfo{item.node_id, static_cast<EleInfo::link_type>(item.static_value * actWeight)});
    }
    return static_cast<size_t>(static_cast<double>(m_node_info.static_value + m_tmp_link_val_) * actWeight);
  }

  /// @brief 将临时连接保存到长期连接中
  /// @param saveWeight 保存权重（要保存的比率）
  void saveTmpLinks(double saveWeight) {
    m_links_.reserve(m_links_.size() + m_links_buffer_.size());

    for (const auto &tmpEle: m_links_buffer_) {
      m_links_.emplace_back(EleInfo{tmpEle.node_id, static_cast<EleInfo::link_type>(tmpEle.static_value * saveWeight)});
    }
    m_node_info.static_value += static_cast<size_t>(static_cast<double>(m_tmp_link_val_) * saveWeight);
  }

  /// @brief 整理所有连接
  /// @param saveStandard 保存标准值（当连接小于标准值时删除连接）
  void tidyAllLink(EleInfo::link_type saveStandard = 0) {
    m_links_.erase(std::find_if(m_links_.begin(), m_links_.end(), [=](const EleInfo &avg) { return avg.static_value < saveStandard; }));
    m_node_info.static_value   = 0;
    m_node_info.static_count = 0;
    std::for_each(m_links_.begin(), m_links_.end(), [&](const EleInfo &avg) {
      m_node_info.static_value += avg.static_value;
      ++m_node_info.static_count;
    });
  }
};*/
#pragma pack(pop)


}// namespace think
#endif//DATA_TYPE_H_

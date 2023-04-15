//
// Created by HanHaocheng on 2023/03/29.
//

#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

#include "type_node_id.h"
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

;

namespace think {


enum class NodeType : unsigned short {
  STATIC,
  DYNAMIC,
  CONST_IN_VISION,
  CONST_OUT_VISION,
};

//==========================================================================================//

#pragma pack(push, 2)

struct LinkInfo {
  using link_type = ushort_t;

  NodeID node_id;
  link_type link_val = 0;
};

using LinkList    = std::vector<LinkInfo>;
using LinkListPtr = std::shared_ptr<LinkList>;

//==========================================================================================//

constexpr static LinkInfo::link_type CONTROL_REMOVE_STANDARD = 1.0;
constexpr static double CONTROL_SAVE_WEIGHT                  = 1.0;


class NodeLink {
public:
  LinkListPtr link;
  size_t link_val;

public:
  explicit NodeLink(size_t linkVal = 0, LinkListPtr linkData = nullptr)
      : link_val(linkVal),
        link(std::move(linkData)) {
  }
  [[nodiscard]] inline bool empty() const { return link_val == 0; }

  void reset(size_t linkValue, const LinkListPtr &linkData) {
    link     = linkData;
    link_val = linkValue;
  }

  template<typename... Args>
  auto &emplace(Args &&...args) {
    return (link->emplace_back(std::forward<Args>(args)...), this->link_val += link->back().link_val, link->back());
  }

  void weaken(double weakenWeight) {
    double remainingWeight = 1.0 - weakenWeight;
    for (auto &lk: *link) {
      lk.link_val = static_cast<LinkInfo::link_type>(lk.link_val * remainingWeight);
    }
    link_val = static_cast<size_t>(static_cast<double>(link_val) * remainingWeight);
  }

  void weaken(LinkInfo::link_type weakenValue) {
    for (auto &lk: *link) {
      lk.link_val -= weakenValue;
    }
    link_val -= weakenValue * link->size();
  }

  void strengthen(double strengthenWeight) {
    double remainingWeight = strengthenWeight + 1.0;
    for (auto &lk: *link) {
      lk.link_val = static_cast<LinkInfo::link_type>(lk.link_val * remainingWeight);
    }
    link_val = static_cast<size_t>(static_cast<double>(link_val) * remainingWeight);
  }

  void strengthen(LinkInfo::link_type strengthenValue) {
    for (auto &lk: *link) {
      lk.link_val += strengthenValue;
    }
    link_val += strengthenValue * link->size();
  }

  void proofread(LinkInfo::link_type saveValue) {
    link->erase(std::find_if(link->begin(),
                             link->end(),
                             [=, this](const LinkInfo &avg) -> bool {
                               return avg.link_val > saveValue;
                             }));
    link_val = 0;
    for (const auto &item: *link) { link_val += item.link_val; }
  }

  void proofread(double saveWeight) {
    proofread(static_cast<LinkInfo::link_type>(static_cast<double>(link_val) * saveWeight));
  }
};

//==========================================================================================//


class Node {
  friend class NodeManageSystem;

  struct LinkList {
    size_t link_val = 0;
    std::vector<LinkInfo> link;
    [[nodiscard]] inline size_t size() const { return link.size(); }
    void clear() {
      link_val = 0;
      link.clear();
    }
    [[nodiscard]] inline bool isEmpty() const { return link_val == 0; }
  };
  struct Link {
    NodeLink const_link;
    NodeLink static_link;
    NodeLink dynamic_link;
    [[nodiscard]] inline bool isEmpty() const { return const_link.empty() && static_link.empty() && dynamic_link.empty(); }
  };
  struct Info {
    NodeType type{};
    size_t const_link_count  = 0;
    size_t static_link_count = 0;
    size_t const_link_val    = 0;
    size_t static_link_val   = 0;
  };

private:
  NodeType node_type = NodeType::DYNAMIC;
  NodeID m_id_{0};

  NodeLink const_link;
  NodeLink static_link;
  NodeLink dynamic_link;

public:
  Node(NodeType nodeType, NodeID id) : node_type(nodeType), m_id_(id), const_link(), static_link(), dynamic_link() {}
  NodeID getId() { return m_id_; }
  NodeType getType() { return node_type; }
  //  const std::shared_ptr<Link> &linkPtr() { return m_links_; }
  [[nodiscard]] inline bool isNull() const { return static_cast<size_t>(m_id_) == 0ULL; }
  [[nodiscard]] inline bool isEmpty() const { return const_link.empty() && static_link.empty() && dynamic_link.empty(); }

  //T = O(n)
  void saveDynamicData() {
    static_link.link->reserve(static_link.link->size() + dynamic_link.link->size());
    for (const auto &dyEle: *dynamic_link.link) {
      auto tmpVal = static_cast<LinkInfo::link_type>(dyEle.link_val * CONTROL_SAVE_WEIGHT);
      if (CONTROL_REMOVE_STANDARD < tmpVal) {
        static_link.emplace(dyEle.node_id, tmpVal);
      }
    }
  }

  bool emplace(NodeType type, LinkInfo links);

  bool emplace(NodeType type, std::vector<LinkInfo> links);
};

using NodePtr     = std::shared_ptr<Node>;
using NodePtrList = std::vector<NodePtr>;
using NodeList    = std::vector<Node>;

/*class Node {
  friend class ThinkCore;

private:
  size_t m_tmp_link_val_ = 0;
  NodeInfo m_node_info;
  std::vector<LinkInfo> m_links_;
  std::vector<LinkInfo> m_links_buffer_;

public:
  [[nodiscard]] NodeID nodeID() const { return m_node_info.type_node_id; }

  void addLink(const LinkInfo &newLink) {
    m_tmp_link_val_ += newLink.static_value;
    m_links_buffer_.push_back(newLink);
  }

  /// @brief 获取激活节点
  /// @param out 返回的激活节点
  /// @param actWeight 激活权重
  /// @return 激活节点总值
  size_t getActLinks (std::vector<LinkInfo> &out, double actWeight) {
    out.reserve(m_node_info.static_count);
    for (const auto &item: m_links_) {
      out.push_back(LinkInfo{item.type_node_id, static_cast<LinkInfo::link_type>(item.static_value * actWeight)});
    }
    for (const auto &item: m_links_buffer_) {
      out.push_back(LinkInfo{item.type_node_id, static_cast<LinkInfo::link_type>(item.static_value * actWeight)});
    }
    return static_cast<size_t>(static_cast<double>(m_node_info.static_value + m_tmp_link_val_) * actWeight);
  }

  /// @brief 将临时连接保存到长期连接中
  /// @param saveWeight 保存权重（要保存的比率）
  void saveTmpLinks(double saveWeight) {
    m_links_.reserve(m_links_.size() + m_links_buffer_.size());

    for (const auto &tmpEle: m_links_buffer_) {
      m_links_.emplace_back(LinkInfo{tmpEle.type_node_id, static_cast<LinkInfo::link_type>(tmpEle.static_value * saveWeight)});
    }
    m_node_info.static_value += static_cast<size_t>(static_cast<double>(m_tmp_link_val_) * saveWeight);
  }

  /// @brief 整理所有连接
  /// @param saveStandard 保存标准值（当连接小于标准值时删除连接）
  void tidyAllLink(LinkInfo::link_type saveStandard = 0) {
    m_links_.erase(std::find_if(m_links_.begin(), m_links_.end(), [=](const LinkInfo &avg) { return avg.static_value < saveStandard; }));
    m_node_info.static_value   = 0;
    m_node_info.static_count = 0;
    std::for_each(m_links_.begin(), m_links_.end(), [&](const LinkInfo &avg) {
      m_node_info.static_value += avg.static_value;
      ++m_node_info.static_count;
    });
  }
};*/
#pragma pack(pop)


}// namespace think


namespace std {
template<>
struct hash<think::NodeID> {
  typedef think::NodeID argument_type;
  typedef std::size_t result_type;
  result_type operator()(argument_type const &s) const {
    return std::hash<size_t>{}(s.operator think::NodeID::id_type());// 或使用 boost::hash_combine （见讨论）
  }
};
}// namespace std

#endif//DATA_TYPE_H_

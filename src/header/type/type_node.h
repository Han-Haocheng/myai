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

namespace think {

enum class NodeType : unsigned short {
  NULL_TYPE,
  DYNAMIC,
  STATIC,
  CONST_IN_VISION,
  CONST_OUT_VISION,
};

const static std::unordered_map<NodeType, std::string> MAP_CONST_TYPE_TO_STRING{
        //        {NodeType::NULL_TYPE, "null_type"},
        //        {NodeType::STATIC, "static"},
        //        {NodeType::DYNAMIC, "dynamic"},
        {NodeType::CONST_IN_VISION, "const_in_vision"},
        {NodeType::CONST_OUT_VISION, "const_out_vision"},
};

//==========================================================================================//

#pragma pack(push, 2)

struct LinkInfo {
  using link_type = ushort_t;

  NodeID node_id;
  link_type link_val = 0;
};


//==========================================================================================//


class LinkList {
public:
  std::vector<LinkInfo> link;
  size_t link_val;

public:
  explicit LinkList(size_t linkVal = 0, std::vector<LinkInfo> &&links = {}) : link_val(linkVal), link(links) {}
  [[nodiscard]] inline bool empty() const { return link_val == 0; }

  void reset(size_t linkValue, std::vector<LinkInfo> &&links = {}) {
    link     = links;
    link_val = linkValue;
  }

  void clear() {
    link.clear();
    link_val = 0;
  }

  template<typename... Args>
  auto &emplace(Args &&...args) {
    return (link.emplace_back(std::forward<Args>(args)...), this->link_val += link.back().link_val, link.back());
  }

  void insert(const LinkList &other) {
    link.insert(link.end(), other.link.begin(), other.link.end());
    link_val += other.link_val;
  }

  void weaken(double weakenWeight) {
    double remainingWeight = 1.0 - weakenWeight;
    for (auto &lk: link) {
      lk.link_val = static_cast<LinkInfo::link_type>(lk.link_val * remainingWeight);
    }
    link_val = static_cast<size_t>(static_cast<double>(link_val) * remainingWeight);
  }

  void weaken(LinkInfo::link_type weakenValue) {
    for (auto &lk: link) {
      lk.link_val -= weakenValue;
    }
    link_val -= weakenValue * link.size();
  }

  void strengthen(double strengthenWeight) {
    double remainingWeight = strengthenWeight + 1.0;
    for (auto &lk: link) {
      lk.link_val = static_cast<LinkInfo::link_type>(lk.link_val * remainingWeight);
    }
    link_val = static_cast<size_t>(static_cast<double>(link_val) * remainingWeight);
  }

  void strengthen(LinkInfo::link_type strengthenValue) {
    for (auto &lk: link) {
      lk.link_val += strengthenValue;
    }
    link_val += strengthenValue * link.size();
  }

  void proofread(LinkInfo::link_type saveValue) {
    link.erase(std::find_if(link.begin(),
                            link.end(),
                            [=](const LinkInfo &avg) -> bool {
                              return avg.link_val > saveValue;
                            }));
    link_val = 0;
    for (const auto &item: link) { link_val += item.link_val; }
  }

  void proofread(double saveWeight) {
    proofread(static_cast<LinkInfo::link_type>(static_cast<double>(link_val) * saveWeight));
  }
};

struct LinkListArray {
  NodeType m_node_type_;
  LinkList m_const_link_;
  LinkList m_static_link_;
  LinkList m_dynamic_link_;
  explicit LinkListArray(NodeType type = NodeType::NULL_TYPE)
      : m_node_type_(type), m_const_link_(), m_static_link_(), m_dynamic_link_() {}
  LinkListArray(NodeType mNodeType, LinkList mConstLink, LinkList mStaticLink, LinkList mDynamicLink)
      : m_node_type_(mNodeType),
        m_const_link_(std::move(mConstLink)),
        m_static_link_(std::move(mStaticLink)),
        m_dynamic_link_(std::move(mDynamicLink)) {
  }
  void saveDynamicData(double saveWeight) {
    m_dynamic_link_.proofread(saveWeight);
    m_static_link_.insert(m_dynamic_link_);
    m_dynamic_link_.clear();
  }
};

//==========================================================================================//


//class Node {
//  //friend class NodeManageSystem;
//
//public:
//  struct link_module {
//    LinkList m_const_link_;
//    LinkList m_static_link_;
//    LinkList m_dynamic_link_;
//  };
//
//private:
//  NodeType node_type;
//  NodeID m_id_;
//  std::shared_ptr<link_module> m_links_;
//
//public:
//  Node(NodeType nodeType, NodeID id,const std::shared_ptr<link_module> &links = nullptr)
//      : node_type(nodeType), m_id_(id), m_links_(links) {}
//  NodeID getId() { return m_id_; }
//  NodeType getType() { return node_type; }
//  const std::shared_ptr<link_module> &getLinks() { return m_links_; }
//  void setLinks(const std::shared_ptr<link_module> &links) { m_links_ = links; }
//  [[nodiscard]] inline bool isNull() const { return static_cast<size_t>(m_id_) == 0ULL; }
//  [[nodiscard]] inline bool isEmpty() const { return m_links_->m_const_link_.empty() && m_links_->m_static_link_.empty() && m_links_.m_dynamic_link_->empty(); }
//
//  //T = O(n)
//  void saveDynamicData() {
//    m_links_->m_dynamic_link_.proofread(CONTROL_SAVE_WEIGHT);
//    m_links_->m_static_link_.insert(m_links_->m_dynamic_link_);
//    m_links_->m_dynamic_link_.clear();
//  }
//
//  bool emplace(NodeType type, LinkInfo links);
//
//  bool insert(NodeType type, const std::vector<LinkInfo> &links);
//};

#pragma pack(pop)


}// namespace think


namespace std {
template<>
struct hash<think::NodeID> {
  typedef think::NodeID argument_type;
  typedef std::size_t result_type;
  result_type operator()(argument_type const &s) const {

    return std::hash<size_t>{}(static_cast<think::NodeID::id_type>(s));// 或使用 boost::hash_combine （见讨论）
  }
};
}// namespace std

#endif//DATA_TYPE_H_

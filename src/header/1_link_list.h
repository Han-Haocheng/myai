//
// Created by HanHaocheng on 2023/05/05.
//

#ifndef THINKSYSTEM_1_LINK_LIST_H
#define THINKSYSTEM_1_LINK_LIST_H
#include "0_think_type.h"
#include <forward_list>

namespace think {

class CLinkList {
  std::forward_list<slink_t> m_lstData_;
  size_t m_iDataSize_;

public:
  using iterator       = std::forward_list<slink_t>::iterator;
  using const_iterator = std::forward_list<slink_t>::const_iterator;
  using pointer        = std::forward_list<slink_t>::pointer;
  using reference      = std::forward_list<slink_t>::reference;

public:
  void resize(size_t s) { m_lstData_.resize(s); }
  pointer data() {
    std::vector<slink_t> buffer(m_iDataSize_);
    std::copy(m_lstData_.begin(), m_lstData_.end(), buffer.begin());
    return buffer.data();
  }
  size_t size() { return m_iDataSize_; }
  void clear() { m_lstData_.clear(); }
  iterator begin() { return m_lstData_.begin(); }
  iterator end() { return m_lstData_.end(); }
  [[nodiscard]] const_iterator begin() const { return m_lstData_.begin(); }
  [[nodiscard]] const_iterator end() const { return m_lstData_.end(); }

  CLinkList &operator+=(int val) {
    for (slink_t &lk: m_lstData_) {
      lk.uiValLink = std::min<int>(NUM_LINK_VAL_MAX, lk.uiValLink + val);
    }
    return *this;
  }

  CLinkList &operator-=(int val) {
    for (slink_t &lk: m_lstData_) {
      lk.uiValLink = std::max<int>(NUM_LINK_VAL_MIN, lk.uiValLink - val);
    }
    return *this;
  }

  CLinkList &operator*=(WEIGHT weight) {
    for (slink_t &lk: m_lstData_) {
      lk.uiValLink = static_cast<link_t>(static_cast<double>(lk.uiValLink) * weight);
    }
    return *this;
  }

  CLinkList operator*(WEIGHT weight) {
    CLinkList res = *this;
    return res *= weight;
  }

  CLinkList operator-(int val) {
    CLinkList res = *this;
    return res -= val;
  }

  CLinkList operator+(int val) {
    CLinkList res = *this;
    return res += val;
  }

  template<typename... Args>
  auto &emplace(Args &&..._args) {
    return m_lstData_.emplace_back(_args...);
  }

  void insert(const CLinkList &other) {
    m_lstData_.insert_after(m_lstData_.end(), other.m_lstData_.begin(), other.m_lstData_.end());
  }

  void erase(link_t standardValue) {
    if (m_iDataSize_ == 0) { return; }
    m_lstData_.remove_if([&](const slink_t &avg) {
      return avg.uiValLink > standardValue && (--m_iDataSize_, true);
    });
  }

  /// @brief ШЅжи
  void deduplicate() {
    std::unordered_set<slink_t> s;
    s.reserve(m_iDataSize_);
    for (slink_t &link: m_lstData_) {
      auto [pNode, bIsAdd] = s.emplace(link);
      if (!bIsAdd) {
        const_cast<link_t &>(pNode->uiValLink) += link.uiValLink;
      }
    }
    m_lstData_.assign(s.begin(), s.end());
  }


};//! struct CLinkList

using active_t     = size_t;
using C_LINK_LIST  = const CLinkList;
using UP_LINK_LIST = std::unique_ptr<CLinkList>;
using LPCLINKLIST  = std::unique_ptr<C_LINK_LIST>;

}// namespace think
#endif//THINKSYSTEM_1_LINK_LIST_H

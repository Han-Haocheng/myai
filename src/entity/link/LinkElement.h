//
// Created by HanHaocheng on 2023/06/10.
//

#ifndef THINK_LINKELEMENT_H
#define THINK_LINKELEMENT_H
#include "../Define.h"

namespace thinksystem::entity
{

#pragma pack(push, 2)
class LinkElement
{
public:
  using id_type   = id_t;
  using link_type = link_t;
  using size_type = size_t;
  using link_pair = ::std::pair<id_type, link_type>;

private:
  id_type m_id_;
  link_type m_val_;

public:
  LinkElement() : m_id_(VAL_DEFINE_ID_TYPE), m_val_(VAL_DEFINE_LINK_TYPE) {}
  LinkElement(const id_type id, link_type lk) : m_id_(id), m_val_(lk) {}
  explicit LinkElement(link_pair lp) : m_id_(lp.first), m_val_(lp.second) {}
  LinkElement(LinkElement &&)      = default;
  LinkElement(const LinkElement &) = default;

public:
  LinkElement &operator=(LinkElement &&)      = default;
  LinkElement &operator=(const LinkElement &) = default;
  LinkElement &operator=(link_pair lp)
  {
    m_id_  = lp.first;
    m_val_ = lp.second;
    return *this;
  }

public:
  friend bool operator==(const LinkElement &lhs, const LinkElement &rhs) { return lhs.m_id_ == rhs.m_id_; }
  friend bool operator==(const LinkElement &lhs, id_type rhs) { return lhs.m_id_ == rhs; }
  friend bool operator==(id_type rhs, const LinkElement &lhs) { return lhs.m_id_ == rhs; }
  friend bool operator!=(const LinkElement &lhs, const LinkElement &rhs) { return lhs.m_id_ != rhs.m_id_; }
  friend bool operator!=(const LinkElement &lhs, id_type rhs) { return lhs.m_id_ != rhs; }
  friend bool operator!=(id_type rhs, const LinkElement &lhs) { return lhs.m_id_ != rhs; }

  LinkElement operator+(link_type rhs) const { return {m_id_, m_val_ + rhs}; }
  LinkElement operator-(link_type rhs) const { return {m_id_, m_val_ + rhs}; }
  LinkElement operator*(const weight_t &rhs) const { return {m_id_, (link_type) ((double) (m_val_) *rhs.linkVal())}; }
  LinkElement &operator+=(link_type rhs) { return *this = (*this + rhs); }
  LinkElement &operator-=(link_type rhs) { return *this = (*this - rhs); }
  LinkElement &operator*=(const weight_t &rhs) { return *this = (*this * rhs); }
  explicit operator link_pair() { return {m_id_, m_val_}; }
  explicit operator byte_t *() { return reinterpret_cast<byte_t *>(this); }
  explicit operator const byte_t *() const { return reinterpret_cast<const byte_t *>(this); }

public:
  void setId(id_type id) { m_id_ = id; }
  void setVal(link_type linkVal) { m_val_ = linkVal; }
  [[nodiscard]] id_type id() const { return m_id_; }
  [[nodiscard]] link_type linkVal() const { return m_val_; }

public:
  [[nodiscard]] bool is_empty(link_type stand = 0) const { return m_val_ == stand; }

public:
};
#pragma pack(pop)

const static LinkElement NULL_LINK = {VAL_DEFINE_ID_TYPE, VAL_DEFINE_LINK_TYPE};
}// namespace thinksystem::entity

#endif//THINK_LINKELEMENT_H

//
// Created by HanHaocheng on 2023/06/02.
//

#ifndef THINK_RANGE_H
#define THINK_RANGE_H
#include <forward_list>
#include <iostream>

#include "SFINAE.h"

namespace thinksystem::utils
{

template<typename Ty, typename = typename std::enable_if<is_arithmetic_extension<Ty>::value>::type>
class Range
{
public:
  using value_type      = Ty;
  using size_type       = size_t;
  using difference_type = ptrdiff_t;
  using pointer         = value_type *;
  using const_pointer   = const value_type *;
  using reference       = value_type &;
  using const_reference = const value_type &;

private:
  value_type m_begin_;
  value_type m_end_;

public:
  Range() : m_begin_(), m_end_() {}
  Range(value_type &&bg, value_type &&ed) : m_begin_(std::min(bg, ed)), m_end_(std::max(bg, ed)) {}
  Range(Range &&) noexcept = default;
  Range(const Range &)     = default;

public:
  Range &operator=(Range &&) noexcept = default;
  Range &operator=(const Range &)     = default;
  bool operator!=(const Range &rhs) const { return m_begin_ != rhs.m_begin_ || m_end_ != rhs.m_end_; }
  bool operator==(const Range &rhs) const { return m_begin_ == rhs.m_begin_ && m_end_ == rhs.m_end_; }

public:
  reference begin() { return m_begin_; }
  const_reference cbegin() const { return m_begin_; }
  void setBegin(const value_type &in) { m_begin_ = in; }
  void setBegin(value_type &&in) { m_begin_ = in; }

  reference end() { return m_end_; }
  const_reference cend() const { return m_end_; }
  void setEnd(const value_type &in) { m_end_ = in; }
  void setEnd(value_type &&in) { m_end_ = in; }

public:
  [[nodiscard]] size_type size() const { return m_end_ - m_begin_; }

  // 判断区间是否为空
  [[nodiscard]] bool is_empty() const { return this->m_begin_ == this->m_end_; }

  // 判断一个值是否在区间内
  bool is_belong(const_reference val) const { return val < m_end_ && val >= m_begin_; }

  // 判断两个区间是否相交
  bool is_intersect(const Range &other) const { return m_begin_ <= other.m_end_ && other.m_begin_ <= m_end_; }

public:
  // 计算两个区间的交集
  [[nodiscard]] Range intersection(const Range &other) const
  {
    if (is_intersect(other)) return {std::max(this->m_begin_, other.begin), std::min(this->m_end_, other.end)};
    return {};
  }

  // 计算两个区间的并集
  [[nodiscard]] Range getUnion(const Range &other) const
  {
    if (is_intersect(other)) return {std::min(this->m_begin_, other.m_begin_), std::max(this->m_end_, other.m_end_)};
    return {};
  }

  bool unionSelf(const Range &other)
  {
    if (!is_intersect(other)) return false;
    this->m_begin_ = std::min(this->m_begin_, other.m_begin_);
    this->m_end_   = std::max(this->m_end_, other.m_end_);
    return true;
  }

  bool intersectionSelf(const Range &other)
  {
    if (!is_intersect(other)) return false;
    this->m_begin_ = std::max(this->m_begin_, other.m_begin_);
    this->m_end_   = std::min(this->m_end_, other.m_end_);
    return true;
  }
};

}// namespace thinksystem::utils

#endif//THINK_RANGE_H

//
// Created by HanHaocheng on 2023/04/10.
//

#ifndef THINKSYSTEM_RANGE_H
#define THINKSYSTEM_RANGE_H

#include <algorithm>
#include <utility>

/// 区间，表示方法[begin,end)
template<typename Ty>
class Range
{
  using data_type       = Ty;
  using reference       = Ty &;
  using size_type       = ptrdiff_t;
  using const_reference = const Ty &;

private:
  data_type m_begin_;
  data_type m_end_;

public:
  Range()                  = default;
  Range(Range &&) noexcept = default;
  Range(const Range &)     = default;
  Range(data_type begin, data_type end) : m_begin_(begin), m_end_(end) {}

  Range &operator=(Range &&) noexcept = default;
  Range &operator=(const Range &)     = default;

  reference begin() { return m_begin_; }
  reference end() { return m_end_; }

  // 判断区间是否为空
  [[nodiscard]] bool is_empty() const { return this->m_begin_ == this->m_end_; }

  // 获取区间长度
  [[nodiscard]] size_type length() const { return this->m_end_ - this->m_begin_; }

  // 判断一个值是否在区间内
  [[nodiscard]] bool contains(const_reference value) const { return value >= this->m_begin_ && value < this->m_end_; }

  // 判断两个区间是否相交
  [[nodiscard]] bool intersects(const Range &other) const
  {
    return this->m_begin_ < other.m_end_ && other.m_begin_ < this->m_end_;
  }

  // 计算两个区间的交集
  [[nodiscard]] Range intersection(const Range &other) const
  {
    int start = std::max(this->m_begin_, other.m_begin_);
    int end   = std::min(this->m_end_, other.m_end_);
    return {start, end};
  }

  // 计算两个区间的并集
  [[nodiscard]] Range merge(const Range &other) const
  {
    int start = std::min(this->m_begin_, other.m_begin_);
    int end   = std::max(this->m_end_, other.m_end_);
    return {start, end};
  }
};

#endif//THINKSYSTEM_RANGE_H

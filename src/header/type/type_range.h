//
// Created by HanHaocheng on 2023/04/10.
//

#ifndef THINKSYSTEM_TYPE_RANGE_H
#define THINKSYSTEM_TYPE_RANGE_H


#include <algorithm>
#include <utility>

//using Ty = int;
template<typename Ty>
class Range : private std::pair<Ty, Ty> {
public:
  Range()                  = default;
  Range(Range &&) noexcept = default;
  Range(const Range &)     = default;

  Range(Ty first, Ty second) : std::pair<Ty, Ty>(first, second) {}

  Range &operator=(Range &&) noexcept = default;
  Range &operator=(const Range &)     = default;

  bool operator<(const Range &rhs) const { return this->first < rhs.first; }
  bool operator>(const Range &rhs) const { return rhs < *this; }
  bool operator>=(const Range &rhs) const { return !(*this < rhs); }
  bool operator<=(const Range &rhs) const { return !(rhs < *this); }

  Ty& begin() { return this->first; }
  Ty& end() { return this->second; }

  // 判断区间是否为空
  [[nodiscard]] bool is_empty() const {
    return this->first == this->second;
  }

  // 获取区间长度
  [[nodiscard]] int length() const {
    return this->second - this->first;
  }

  // 判断一个值是否在区间内
  [[nodiscard]] bool contains(const Ty &value) const {
    return value >= this->first && value < this->second;
  }

  // 判断两个区间是否相交
  [[nodiscard]] bool intersects(const Range &other) const {
    return this->first < other.second && other.first < this->second;
  }

  // 计算两个区间的交集
  [[nodiscard]] Range intersection(const Range &other) const {
    int start = std::max(this->first, other.first);
    int end   = std::min(this->second, other.second);
    return {start, end};
  }

  // 计算两个区间的并集
  [[nodiscard]] Range merge(const Range &other) const {
    int start = std::min(this->first, other.first);
    int end   = std::max(this->second, other.second);
    return {start, end};
  }
};


#endif//THINKSYSTEM_TYPE_RANGE_H

//
// Created by HanHaocheng on 2023/06/07.
//

#ifndef THINK_BOUNDEDVALUE_H
#define THINK_BOUNDEDVALUE_H
#include <type_traits>

namespace thinksystem::utils
{

/// This class is dakaideyeshu value with bounded.it can have add
/// \tparam Ty
template<typename Ty, std::uint64_t MIN, std::uint64_t MAX, typename = std::enable_if<std::is_arithmetic_v<Ty>>>
class BoundedValue
{
  static_assert(MIN < MAX, "error:min less max");

private:
  using self = BoundedValue;

public:
  using value_type = Ty;
  using references = Ty&;
  using const_ref  = const Ty&;

public:
private:
  value_type m_val_;

public:
  static constexpr value_type LIMIT(const value_type& linkVal) noexcept { return linkVal < MIN ? MIN : linkVal > MAX ? MAX : linkVal; };

public:
  BoundedValue() : m_val_() {}
  BoundedValue(const_ref linkVal) : m_val_(LIMIT(linkVal)) {}
  BoundedValue(self&&) noexcept = default;
  BoundedValue(const self&)     = default;

public:
  self& operator=(const_ref linkVal) { m_val_ = LIMIT(linkVal); }
  self& operator=(self&&) noexcept = default;
  self& operator=(const self&)     = default;

  friend self operator+(const self& lhs, const self& rhs) noexcept { return {LIMIT(lhs.m_val_ + rhs.m_val_)}; }
  friend self operator-(const self& lhs, const self& rhs) noexcept { return {LIMIT(lhs.m_val_ - rhs.m_val_)}; }
  friend self operator*(const self& lhs, const self& rhs) noexcept { return {LIMIT(lhs.m_val_ * rhs.m_val_)}; }
  friend self operator/(const self& lhs, const self& rhs) noexcept { return {LIMIT(lhs.m_val_ / rhs.m_val_)}; }
  friend self& operator+=(self& lhs, const self& rhs) noexcept { return lhs = lhs + rhs; }
  friend self& operator-=(self& lhs, const self& rhs) noexcept { return lhs = lhs - rhs; }
  friend self& operator*=(self& lhs, const self& rhs) noexcept { return lhs = lhs * rhs; }
  friend self& operator/=(self& lhs, const self& rhs) noexcept { return lhs = lhs / rhs; }
  friend self operator+(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ + rhs)}; }
  friend self operator-(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ - rhs)}; }
  friend self operator*(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ * rhs)}; }
  friend self operator/(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ / rhs)}; }

  friend bool operator==(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ == rhs.m_val_; }
  friend bool operator!=(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ != rhs.m_val_; }
  friend bool operator<(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ < rhs.m_val_; }
  friend bool operator>(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ > rhs.m_val_; }
  friend bool operator<=(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ <= rhs.m_val_; }
  friend bool operator>=(const self& lhs, const self& rhs) noexcept { return lhs.m_val_ >= rhs.m_val_; }

  friend bool operator==(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ == rhs; }
  friend bool operator!=(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ != rhs; }
  friend bool operator<(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ < rhs; }
  friend bool operator>(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ > rhs; }
  friend bool operator<=(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ <= rhs; }
  friend bool operator>=(const self& lhs, const_ref rhs) noexcept { return lhs.m_val_ >= rhs; }

  friend bool operator==(const_ref lhs, const self& rhs) noexcept { return lhs == rhs.m_val_; }
  friend bool operator!=(const_ref lhs, const self& rhs) noexcept { return lhs != rhs.m_val_; }
  friend bool operator<(const_ref lhs, const self& rhs) noexcept { return lhs < rhs.m_val_; }
  friend bool operator>(const_ref lhs, const self& rhs) noexcept { return lhs > rhs.m_val_; }
  friend bool operator<=(const_ref lhs, const self& rhs) noexcept { return lhs <= rhs.m_val_; }
  friend bool operator>=(const_ref lhs, const self& rhs) noexcept { return lhs >= rhs.m_val_; }

public:
  inline value_type linkVal() const { return m_val_; }
  inline const_ref ref() const { return m_val_; }
  inline references ref() { return m_val_; }

public:
  [[nodiscard]] inline bool isMax() const { return m_val_ == MAX; }
  [[nodiscard]] inline bool isMin() const { return m_val_ == MIN; }
};//! class BoundedValue

}// namespace thinksystem::utils

#endif//THINK_BOUNDEDVALUE_H

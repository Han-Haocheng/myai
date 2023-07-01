//
// Created by HanHaocheng on 2023/06/07.
//

#ifndef THINK_BOUNDEDVALUE_H
#define THINK_BOUNDEDVALUE_H
#include <type_traits>

#define CXX03 199711L
#define CXX11 201103L
#define CXX14 201402L
#define CXX17 201703L
#define CXX20 202002L

#if defined(__clang__) || defined(__GNUC__)
#define CPP_STANDARD __cplusplus
#elif defined(_MSC_VER)
#define CPP_STANDARD _MSVC_LANG
#endif

#if CPP_STANDARD >= CXX03
#define HAS_CPP_03 true
#endif

#if CPP_STANDARD >= CXX11
#define HAS_CPP_11 true
#endif

#if CPP_STANDARD >= CXX14
#define HAS_CPP_14 true
#endif

#if CPP_STANDARD >= CXX17
#define HAS_CPP_17 true
#endif

#if CPP_STANDARD >= CXX20
#define HAS_CPP_20 true
#endif

namespace thinksystem::utils
{
//using Ty                                     = int;
//static const constexpr std::uint64_t MIN_VAL = 0, MAX_VAL = 10;

/// This class is dakaideyeshu value with bounded.it can have add
/// \tparam Ty
template<typename Ty, std::int64_t MIN_VAL, std::int64_t MAX_VAL, typename = std::enable_if<std::is_arithmetic_v<Ty>>>
class BoundedValue
{
  static_assert(MIN_VAL < MAX_VAL, "error:min less max");

private:
  using self = BoundedValue;

public:
  using value_type = Ty;
  using references = Ty&;
  using const_ref  = const Ty&;

public:
  static const value_type MAX = static_cast<value_type>(MAX_VAL);
  static const value_type MIN = static_cast<value_type>(MIN_VAL);

private:
  value_type m_val_;

public:
  static constexpr value_type LIMIT(const value_type& linkVal)
  {
    return linkVal < MIN ? MIN : linkVal > MAX ? MAX : linkVal;
  }

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

#if CPP_STANDARD < CXX20
  friend self operator+(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ + rhs)}; }
  friend self operator-(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ - rhs)}; }
  friend self operator*(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ * rhs)}; }
  friend self operator/(const self& lhs, const_ref rhs) noexcept { return {LIMIT(lhs.m_val_ / rhs)}; }
#endif// _HAS_CXX20

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
  inline value_type val() const { return m_val_; }
  inline const_ref ref() const { return m_val_; }
  inline references ref() { return m_val_; }

public:
  [[nodiscard]] inline bool isMax() const { return m_val_ == MAX; }
  [[nodiscard]] inline bool isMin() const { return m_val_ == MIN; }
};//! class BoundedValue

}// namespace thinksystem::utils

#endif//THINK_BOUNDEDVALUE_H

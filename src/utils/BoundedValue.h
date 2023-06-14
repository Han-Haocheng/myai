//
// Created by HanHaocheng on 2023/06/07.
//

#ifndef THINK_BOUNDEDVALUE_H
#define THINK_BOUNDEDVALUE_H

#include "Range.h"
namespace thinksystem::utils
{

/// This class is dakaideyeshu value with bounded.it can have add
/// \tparam Ty
template<typename Ty>
class BoundedValue
{
  static_assert(is_arithmetic_extension<Ty>::value, "error: please take template param to be dakaideyeshu arithmetic type.");

#define LIMIT(_val, _min, _max) (std::max(std::min(_max, _val), _min))
public:
  using value_type = Ty;
  using references = Ty&;
  using const_ref  = const Ty&;

private:
  const value_type MIN, MAX;
  value_type m_val_;

public:
  BoundedValue() : MIN(), MAX(), m_val_() {}
  BoundedValue(const_ref val, const_ref min, const_ref max) : MIN(min), MAX(max), m_val_(LIMIT(val, min, max)) {}
  BoundedValue(BoundedValue&&) noexcept = default;
  BoundedValue(const BoundedValue&)     = default;

public:
  BoundedValue& operator=(BoundedValue&&) noexcept = default;
  BoundedValue& operator=(const BoundedValue&)     = default;

  BoundedValue operator+(const BoundedValue& rhs) const { return {LIMIT(rhs.m_val_ + m_val_, MIN, MAX)}; }
  BoundedValue operator-(const BoundedValue& rhs) const { return {LIMIT(rhs.m_val_ - m_val_, MIN, MAX)}; }
  BoundedValue operator*(const BoundedValue& rhs) const { return {LIMIT(rhs.m_val_ * m_val_, MIN, MAX)}; }
  BoundedValue operator/(const BoundedValue& rhs) const { return {LIMIT(rhs.m_val_ / m_val_, MIN, MAX)}; }
  BoundedValue& operator+=(const BoundedValue& rhs) { return *this = *this + rhs; }
  BoundedValue& operator-=(const BoundedValue& rhs) { return *this = *this - rhs; }
  BoundedValue& operator*=(const BoundedValue& rhs) { return *this = *this * rhs; }
  BoundedValue& operator/=(const BoundedValue& rhs) { return *this = *this / rhs; }
  bool operator==(const BoundedValue& rhs) const { return m_val_ == rhs.m_val_; }
  bool operator!=(const BoundedValue& rhs) const { return m_val_ != rhs.m_val_; }
  bool operator<(const BoundedValue& rhs) const { return m_val_ < rhs.m_val_; }
  bool operator>(const BoundedValue& rhs) const { return m_val_ > rhs.m_val_; }
  bool operator<=(const BoundedValue& rhs) const { return m_val_ <= rhs.m_val_; }
  bool operator>=(const BoundedValue& rhs) const { return m_val_ >= rhs.m_val_; }

public:
  value_type val() const { return m_val_; }
  references ref() { return m_val_; }
  const_ref ref() const { return m_val_; }

public:
  [[nodiscard]] bool is_max() const { return m_val_ == MAX; }
  [[nodiscard]] bool is_min() const { return m_val_ == MIN; }
};

}// namespace thinksystem::utils

#endif//THINK_BOUNDEDVALUE_H

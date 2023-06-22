//
// Created by HanHaocheng on 2023/06/08.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"

#ifndef THINK_SFINAE_H
#define THINK_SFINAE_H
#include <type_traits>

namespace thinksystem::utils
{

/// @check
template<typename T>
struct is_arithmetic_extension
{
  template<typename U>
  static auto checkAddition(int) -> decltype(std::declval<U>() + std::declval<U>(), std::true_type());
  template<typename U>
  static auto checkSubtraction(int) -> decltype(std::declval<U>() - std::declval<U>(), std::true_type());
  template<typename U>
  static auto checkMultiplication(int) -> decltype(std::declval<U>() * std::declval<U>(), std::true_type());
  template<typename U>
  static auto checkDivision(int) -> decltype(std::declval<U>() / std::declval<U>(), std::true_type());

  template<typename>
  static std::false_type checkAddition(...);
  template<typename>
  static std::false_type checkSubtraction(...);
  template<typename>
  static std::false_type checkMultiplication(...);
  template<typename>
  static std::false_type checkDivision(...);

  static constexpr bool value = std::is_same<decltype(checkAddition<T>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkSubtraction<T>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkMultiplication<T>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkDivision<T>(0)), std::true_type>::value;
};

template<typename ValueTy, template<typename> class ContainerTy>
struct is_container
{
  template<typename U>
  static auto checkBegin(int) -> decltype(*std::declval<U>().cbegin(), std::true_type());
  template<typename U>
  static auto checkEnd(int) -> decltype(*std::declval<U>().cend(), std::true_type());

  template<typename U>
  static auto checkEqual(int) -> decltype(std::declval<U>().cend() == std::declval<U>().cbegin(), std::true_type());
  template<typename U>
  static auto checkUnequal(int) -> decltype(std::declval<U>().cend() != std::declval<U>().cbegin(), std::true_type());
  template<typename>
  static std::false_type checkBegin(...);
  template<typename>
  static std::false_type checkEnd(...);
  template<typename>
  static std::false_type checkEqual(...);
  template<typename>
  static std::false_type checkUnequal(...);

  static constexpr bool value = std::is_same<decltype(checkBegin<ContainerTy<ValueTy>>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkEnd<ContainerTy<ValueTy>>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkEqual<ContainerTy<ValueTy>>(0)), std::true_type>::value &&
                                std::is_same<decltype(checkUnequal<ContainerTy<ValueTy>>(0)), std::true_type>::value;
};

}// namespace thinksystem::utils

#endif//THINK_SFINAE_H

#pragma clang diagnostic pop
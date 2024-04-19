#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H
#include "../core/mylib_def.h"

MYLIB_SPACE_BEGIN

template<typename FirstTy, typename SecondTy>
class Converter {
public:
  static_assert(!std::is_same<FirstTy, SecondTy>::value, "错误：模板参数转换类型相同，无需转换");

  SecondTy cast(const FirstTy &from) {
    return boost::lexical_cast<SecondTy>(from);
  }

  FirstTy cast(const SecondTy &from) {
    return boost::lexical_cast<FirstTy>(from);
  }
};

template<typename Ty>
class StringConverter : private Converter<Ty, String> {
public:
  virtual String toString(const Ty &ty) { return Converter<Ty, String>::cast(ty); }

  virtual Ty fromString(const String &ty) { return Converter<Ty, String>::cast(ty); }
};

template<>
class StringConverter<String> {
public:
  virtual String toString(const String &ty) { return ty; }
  virtual String fromString(const String &ty) { return ty; }
};

template<typename U>
struct is_converter {
  template<typename T, void (T::*)() = &T::toString>
  static constexpr bool checkToString(T *) { return true; }
  static constexpr bool checkToString(...) { return false; }

  template<typename T, void (T::*)() = &T::fromString>
  static constexpr bool checkFromString(T *) { return true; }
  static constexpr bool checkFromString(...) { return false; }
  static constexpr bool value = checkToString(static_cast<U *>(0)) && checkFromString(static_cast<U *>(0));
};

MYLIB_SPACE_END
#endif//MYLIB_CONVERTER_H

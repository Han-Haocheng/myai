#pragma once
#include "../core/mylib_def.h"
#include <ctime>
#include <iostream>

MYLIB_SPACE_BEGIN

extern "C" inline time_t GetCurrentTimestamp() {
  return time(NULL);
}

extern "C" inline String FormatClassName(CString str) {
  if (!str) {
    return "";
  }
  String tmpStr = str;
  size_t pos = tmpStr.find_last_of(' ') + 1, n = tmpStr.find_last_of(':') - pos - 1;
  auto res = tmpStr.substr(pos, n);
  //std::cout << str << std::endl;
  return res;
}

#define _PRETTY_FUNC_ __PRETTY_##FUNCTION__
#define __CLASS_NAME__ MYLIB_SPACE_NAME::FormatClassName(_PRETTY_FUNC_)

constexpr size_t string_length(CString str) {
  size_t a = 0;
  for (;;)
    if (str[a++] == '\0') return a;
  return 0;
}

constexpr char ToUpper(char c) {
  return c < 'a' || c > 'z' ? c : c - 32;
}



#define TO_UPPER_BASE(n, str) MYLIB_SPACE_NAME::ToUpper(str[n])
#ifdef TO_UPPER_BASE
  #define TO_UPPER_BASE_0(str) 0
  #define TO_UPPER_BASE_1(str) TO_UPPER_BASE(0, str)
  #define TO_UPPER_BASE_2(str) TO_UPPER_BASE_1(str), TO_UPPER_BASE(1, str)
  #define TO_UPPER_BASE_3(str) TO_UPPER_BASE_2(str), TO_UPPER_BASE(2, str)
  #define TO_UPPER_BASE_4(str) TO_UPPER_BASE_3(str), TO_UPPER_BASE(3, str)
  #define TO_UPPER_BASE_5(str) TO_UPPER_BASE_4(str), TO_UPPER_BASE(4, str)
  #define TO_UPPER_BASE_6(str) TO_UPPER_BASE_5(str), TO_UPPER_BASE(5, str)
  #define TO_UPPER_BASE_7(str) TO_UPPER_BASE_6(str), TO_UPPER_BASE(6, str)
  #define TO_UPPER_BASE_8(str) TO_UPPER_BASE_7(str), TO_UPPER_BASE(7, str)
  #define TO_UPPER_BASE_9(str) TO_UPPER_BASE_8(str), TO_UPPER_BASE(8, str)
  #define TO_UPPER_BASE_10(str) TO_UPPER_BASE_9(str), TO_UPPER_BASE(9, str)
  #define TO_UPPER_BASE_11(str) TO_UPPER_BASE_10(str), TO_UPPER_BASE(10, str)
  #define TO_UPPER_BASE_12(str) TO_UPPER_BASE_11(str), TO_UPPER_BASE(11, str)
  #define TO_UPPER_BASE_13(str) TO_UPPER_BASE_12(str), TO_UPPER_BASE(12, str)
  #define TO_UPPER_BASE_14(str) TO_UPPER_BASE_13(str), TO_UPPER_BASE(13, str)
  #define TO_UPPER_BASE_15(str) TO_UPPER_BASE_14(str), TO_UPPER_BASE(14, str)
  #define TO_UPPER_BASE_16(str) TO_UPPER_BASE_15(str), TO_UPPER_BASE(15, str)
  #define TO_UPPER_BASE_17(str) TO_UPPER_BASE_16(str), TO_UPPER_BASE(16, str)
  #define TO_UPPER_BASE_18(str) TO_UPPER_BASE_17(str), TO_UPPER_BASE(17, str)
  #define TO_UPPER_BASE_19(str) TO_UPPER_BASE_18(str), TO_UPPER_BASE(18, str)
  #define TO_UPPER_BASE_20(str) TO_UPPER_BASE_19(str), TO_UPPER_BASE(19, str)
  #define TO_UPPER_BASE_21(str) TO_UPPER_BASE_20(str), TO_UPPER_BASE(20, str)
  #define TO_UPPER_BASE_22(str) TO_UPPER_BASE_21(str), TO_UPPER_BASE(21, str)
  #define TO_UPPER_BASE_23(str) TO_UPPER_BASE_22(str), TO_UPPER_BASE(22, str)
  #define TO_UPPER_BASE_24(str) TO_UPPER_BASE_23(str), TO_UPPER_BASE(23, str)
  #define TO_UPPER_BASE_25(str) TO_UPPER_BASE_24(str), TO_UPPER_BASE(24, str)
  #define TO_UPPER_BASE_26(str) TO_UPPER_BASE_25(str), TO_UPPER_BASE(25, str)
  #define TO_UPPER_BASE_27(str) TO_UPPER_BASE_26(str), TO_UPPER_BASE(26, str)
  #define TO_UPPER_BASE_28(str) TO_UPPER_BASE_27(str), TO_UPPER_BASE(27, str)
  #define TO_UPPER_BASE_29(str) TO_UPPER_BASE_28(str), TO_UPPER_BASE(28, str)
  #define TO_UPPER_BASE_30(str) TO_UPPER_BASE_29(str), TO_UPPER_BASE(29, str)
  #define TO_UPPER_BASE_31(str) TO_UPPER_BASE_30(str), TO_UPPER_BASE(30, str)
  #define TO_UPPER_BASE_32(str) TO_UPPER_BASE_31(str), TO_UPPER_BASE(31, str)
  #define TO_UPPER_BASE_33(str) TO_UPPER_BASE_32(str), TO_UPPER_BASE(32, str)
  #define TO_UPPER_BASE_34(str) TO_UPPER_BASE_33(str), TO_UPPER_BASE(33, str)
  #define TO_UPPER_BASE_35(str) TO_UPPER_BASE_34(str), TO_UPPER_BASE(34, str)
  #define TO_UPPER_BASE_36(str) TO_UPPER_BASE_35(str), TO_UPPER_BASE(35, str)
  #define TO_UPPER_BASE_37(str) TO_UPPER_BASE_36(str), TO_UPPER_BASE(36, str)
  #define TO_UPPER_BASE_38(str) TO_UPPER_BASE_37(str), TO_UPPER_BASE(37, str)
  #define TO_UPPER_BASE_39(str) TO_UPPER_BASE_38(str), TO_UPPER_BASE(38, str)
  #define TO_UPPER_BASE_40(str) TO_UPPER_BASE_39(str), TO_UPPER_BASE(39, str)
  #define TO_UPPER_BASE_41(str) TO_UPPER_BASE_40(str), TO_UPPER_BASE(40, str)
  #define TO_UPPER_BASE_42(str) TO_UPPER_BASE_41(str), TO_UPPER_BASE(41, str)
  #define TO_UPPER_BASE_43(str) TO_UPPER_BASE_42(str), TO_UPPER_BASE(42, str)
  #define TO_UPPER_BASE_44(str) TO_UPPER_BASE_43(str), TO_UPPER_BASE(43, str)
  #define TO_UPPER_BASE_45(str) TO_UPPER_BASE_44(str), TO_UPPER_BASE(44, str)
  #define TO_UPPER_BASE_46(str) TO_UPPER_BASE_45(str), TO_UPPER_BASE(45, str)
  #define TO_UPPER_BASE_47(str) TO_UPPER_BASE_46(str), TO_UPPER_BASE(46, str)
  #define TO_UPPER_BASE_48(str) TO_UPPER_BASE_47(str), TO_UPPER_BASE(47, str)
  #define TO_UPPER_BASE_49(str) TO_UPPER_BASE_48(str), TO_UPPER_BASE(48, str)
  #define TO_UPPER_BASE_50(str) TO_UPPER_BASE_49(str), TO_UPPER_BASE(49, str)
  #define TO_UPPER_BASE_51(str) TO_UPPER_BASE_50(str), TO_UPPER_BASE(50, str)
  #define TO_UPPER_BASE_52(str) TO_UPPER_BASE_51(str), TO_UPPER_BASE(51, str)
  #define TO_UPPER_BASE_53(str) TO_UPPER_BASE_52(str), TO_UPPER_BASE(52, str)
  #define TO_UPPER_BASE_54(str) TO_UPPER_BASE_53(str), TO_UPPER_BASE(53, str)
  #define TO_UPPER_BASE_55(str) TO_UPPER_BASE_54(str), TO_UPPER_BASE(54, str)
  #define TO_UPPER_BASE_56(str) TO_UPPER_BASE_55(str), TO_UPPER_BASE(55, str)
  #define TO_UPPER_BASE_57(str) TO_UPPER_BASE_56(str), TO_UPPER_BASE(56, str)
  #define TO_UPPER_BASE_58(str) TO_UPPER_BASE_57(str), TO_UPPER_BASE(57, str)
  #define TO_UPPER_BASE_59(str) TO_UPPER_BASE_58(str), TO_UPPER_BASE(58, str)
  #define TO_UPPER_BASE_60(str) TO_UPPER_BASE_59(str), TO_UPPER_BASE(59, str)
  #define TO_UPPER_BASE_61(str) TO_UPPER_BASE_60(str), TO_UPPER_BASE(60, str)
  #define TO_UPPER_BASE_62(str) TO_UPPER_BASE_61(str), TO_UPPER_BASE(61, str)
  #define TO_UPPER_BASE_63(str) TO_UPPER_BASE_62(str), TO_UPPER_BASE(62, str)
#endif

constexpr char ToLower(char c) {
  return c < 'A' || c > 'Z' ? c : c + 32;
}

#define TO_LOWER(n, str) TO_LOWER_BASE_##n(str)
#define TO_LOWER_BASE(n, str) MYLIB_SPACE_NAME::ToLower(str[n])

#ifdef TO_LOWER_BASE
  #define TO_LOWER_BASE_0(str) 0
  #define TO_LOWER_BASE_1(str) TO_LOWER_BASE(0, str)
  #define TO_LOWER_BASE_2(str) TO_LOWER_BASE_1(str), TO_LOWER_BASE(1, str)
  #define TO_LOWER_BASE_3(str) TO_LOWER_BASE_2(str), TO_LOWER_BASE(2, str)
  #define TO_LOWER_BASE_4(str) TO_LOWER_BASE_3(str), TO_LOWER_BASE(3, str)
  #define TO_LOWER_BASE_5(str) TO_LOWER_BASE_4(str), TO_LOWER_BASE(4, str)
  #define TO_LOWER_BASE_6(str) TO_LOWER_BASE_5(str), TO_LOWER_BASE(5, str)
  #define TO_LOWER_BASE_7(str) TO_LOWER_BASE_6(str), TO_LOWER_BASE(6, str)
  #define TO_LOWER_BASE_8(str) TO_LOWER_BASE_7(str), TO_LOWER_BASE(7, str)
  #define TO_LOWER_BASE_9(str) TO_LOWER_BASE_8(str), TO_LOWER_BASE(8, str)
  #define TO_LOWER_BASE_10(str) TO_LOWER_BASE_9(str), TO_LOWER_BASE(9, str)
  #define TO_LOWER_BASE_11(str) TO_LOWER_BASE_10(str), TO_LOWER_BASE(10, str)
  #define TO_LOWER_BASE_12(str) TO_LOWER_BASE_11(str), TO_LOWER_BASE(11, str)
  #define TO_LOWER_BASE_13(str) TO_LOWER_BASE_12(str), TO_LOWER_BASE(12, str)
  #define TO_LOWER_BASE_14(str) TO_LOWER_BASE_13(str), TO_LOWER_BASE(13, str)
  #define TO_LOWER_BASE_15(str) TO_LOWER_BASE_14(str), TO_LOWER_BASE(14, str)
  #define TO_LOWER_BASE_16(str) TO_LOWER_BASE_15(str), TO_LOWER_BASE(15, str)
  #define TO_LOWER_BASE_17(str) TO_LOWER_BASE_16(str), TO_LOWER_BASE(16, str)
  #define TO_LOWER_BASE_18(str) TO_LOWER_BASE_17(str), TO_LOWER_BASE(17, str)
  #define TO_LOWER_BASE_19(str) TO_LOWER_BASE_18(str), TO_LOWER_BASE(18, str)
  #define TO_LOWER_BASE_20(str) TO_LOWER_BASE_19(str), TO_LOWER_BASE(19, str)
  #define TO_LOWER_BASE_21(str) TO_LOWER_BASE_20(str), TO_LOWER_BASE(20, str)
  #define TO_LOWER_BASE_22(str) TO_LOWER_BASE_21(str), TO_LOWER_BASE(21, str)
  #define TO_LOWER_BASE_23(str) TO_LOWER_BASE_22(str), TO_LOWER_BASE(22, str)
  #define TO_LOWER_BASE_24(str) TO_LOWER_BASE_23(str), TO_LOWER_BASE(23, str)
  #define TO_LOWER_BASE_25(str) TO_LOWER_BASE_24(str), TO_LOWER_BASE(24, str)
  #define TO_LOWER_BASE_26(str) TO_LOWER_BASE_25(str), TO_LOWER_BASE(25, str)
  #define TO_LOWER_BASE_27(str) TO_LOWER_BASE_26(str), TO_LOWER_BASE(26, str)
  #define TO_LOWER_BASE_28(str) TO_LOWER_BASE_27(str), TO_LOWER_BASE(27, str)
  #define TO_LOWER_BASE_29(str) TO_LOWER_BASE_28(str), TO_LOWER_BASE(28, str)
  #define TO_LOWER_BASE_30(str) TO_LOWER_BASE_29(str), TO_LOWER_BASE(29, str)
  #define TO_LOWER_BASE_31(str) TO_LOWER_BASE_30(str), TO_LOWER_BASE(30, str)
  #define TO_LOWER_BASE_32(str) TO_LOWER_BASE_31(str), TO_LOWER_BASE(31, str)
  #define TO_LOWER_BASE_33(str) TO_LOWER_BASE_32(str), TO_LOWER_BASE(32, str)
  #define TO_LOWER_BASE_34(str) TO_LOWER_BASE_33(str), TO_LOWER_BASE(33, str)
  #define TO_LOWER_BASE_35(str) TO_LOWER_BASE_34(str), TO_LOWER_BASE(34, str)
  #define TO_LOWER_BASE_36(str) TO_LOWER_BASE_35(str), TO_LOWER_BASE(35, str)
  #define TO_LOWER_BASE_37(str) TO_LOWER_BASE_36(str), TO_LOWER_BASE(36, str)
  #define TO_LOWER_BASE_38(str) TO_LOWER_BASE_37(str), TO_LOWER_BASE(37, str)
  #define TO_LOWER_BASE_39(str) TO_LOWER_BASE_38(str), TO_LOWER_BASE(38, str)
  #define TO_LOWER_BASE_40(str) TO_LOWER_BASE_39(str), TO_LOWER_BASE(39, str)
  #define TO_LOWER_BASE_41(str) TO_LOWER_BASE_40(str), TO_LOWER_BASE(40, str)
  #define TO_LOWER_BASE_42(str) TO_LOWER_BASE_41(str), TO_LOWER_BASE(41, str)
  #define TO_LOWER_BASE_43(str) TO_LOWER_BASE_42(str), TO_LOWER_BASE(42, str)
  #define TO_LOWER_BASE_44(str) TO_LOWER_BASE_43(str), TO_LOWER_BASE(43, str)
  #define TO_LOWER_BASE_45(str) TO_LOWER_BASE_44(str), TO_LOWER_BASE(44, str)
  #define TO_LOWER_BASE_46(str) TO_LOWER_BASE_45(str), TO_LOWER_BASE(45, str)
  #define TO_LOWER_BASE_47(str) TO_LOWER_BASE_46(str), TO_LOWER_BASE(46, str)
  #define TO_LOWER_BASE_48(str) TO_LOWER_BASE_47(str), TO_LOWER_BASE(47, str)
  #define TO_LOWER_BASE_49(str) TO_LOWER_BASE_48(str), TO_LOWER_BASE(48, str)
  #define TO_LOWER_BASE_50(str) TO_LOWER_BASE_49(str), TO_LOWER_BASE(49, str)
  #define TO_LOWER_BASE_51(str) TO_LOWER_BASE_50(str), TO_LOWER_BASE(50, str)
  #define TO_LOWER_BASE_52(str) TO_LOWER_BASE_51(str), TO_LOWER_BASE(51, str)
  #define TO_LOWER_BASE_53(str) TO_LOWER_BASE_52(str), TO_LOWER_BASE(52, str)
  #define TO_LOWER_BASE_54(str) TO_LOWER_BASE_53(str), TO_LOWER_BASE(53, str)
  #define TO_LOWER_BASE_55(str) TO_LOWER_BASE_54(str), TO_LOWER_BASE(54, str)
  #define TO_LOWER_BASE_56(str) TO_LOWER_BASE_55(str), TO_LOWER_BASE(55, str)
  #define TO_LOWER_BASE_57(str) TO_LOWER_BASE_56(str), TO_LOWER_BASE(56, str)
  #define TO_LOWER_BASE_58(str) TO_LOWER_BASE_57(str), TO_LOWER_BASE(57, str)
  #define TO_LOWER_BASE_59(str) TO_LOWER_BASE_58(str), TO_LOWER_BASE(58, str)
  #define TO_LOWER_BASE_60(str) TO_LOWER_BASE_59(str), TO_LOWER_BASE(59, str)
  #define TO_LOWER_BASE_61(str) TO_LOWER_BASE_60(str), TO_LOWER_BASE(60, str)
  #define TO_LOWER_BASE_62(str) TO_LOWER_BASE_61(str), TO_LOWER_BASE(61, str)
  #define TO_LOWER_BASE_63(str) TO_LOWER_BASE_62(str), TO_LOWER_BASE(62, str)
#endif

#define TO_LOWER(n, str) TO_LOWER_BASE_##n(str)

MYLIB_SPACE_END
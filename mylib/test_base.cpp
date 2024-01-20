//
// Created by HanHaocheng on 2023/12/28.
//

#define BOOST_TEST_MODULE MyLibTest
#include <boost/test/unit_test.hpp>
#include <utility>

BOOST_AUTO_TEST_SUITE(suite_mylib_base)

class Person
{
public:
  Person(std::string name, int age) : m_name(std::move(name)), m_age(age) {}
  const std::string &getName() const
  {
    return m_name;
  }
  void setName(const std::string &name)
  {
    Person::m_name = name;
  }
  int getAge() const
  {
    return m_age;
  }
  void setAge(int age)
  {
    Person::m_age = age;
  }

private:
  std::string m_name;
  int m_age;
};

BOOST_AUTO_TEST_CASE(test_base)
{

  int a = 0, b = 0, c = 0;
  printf("输入第一门成绩：");
  scanf("%d", &a);
  printf("输入第二门成绩：");
  scanf("%d", &b);
  printf("输入第三门成绩：");
  scanf("%d", &c);
  printf("平均分是：%d", ((a + b + c) / 3));



  printf("请输入日期：");
  int a;
  scanf("%d", &a);
  if (a >= 1 && a <= 5) {
    printf("工作日");
  } else if (a >= 6 && a <= 7) {
    printf("休息日");
  } else {
    printf("输入错误");
  }



  printf("请输入x值：");
  int x = 0;
  scanf("%d", &x);
  if (x < 0) {
    printf("%d", x + 1);
  } else if (x > 0) {
    printf("%d", x - 1);
  } else {
    printf("%d", 0);
  }



  std::cout << ("请输入一个三位数：");
  int x = 0;
  std::cin >> x;
  if (x < 100 || x > 999) {
    printf("超出范围");
    return;
  }
  int b = x / 100;
  int g = x % 10;
  x = x - g + b - (b * 100) + (g * 100);
  printf("%d", x);
}

BOOST_AUTO_TEST_SUITE_END()
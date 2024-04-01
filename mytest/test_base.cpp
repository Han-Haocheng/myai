//
// Created by HanHaocheng on 2023/12/28.
//

#define BOOST_TEST_MODULE MyLibTest
#include <boost/test/unit_test.hpp>
#include <chrono>
#include <random>

BOOST_AUTO_TEST_SUITE(suite_mylib_base)

BOOST_AUTO_TEST_CASE(test_base)
{
  std::vector<int> vec = {1, 2, 3, 4, 5};

  for (auto iterator = vec.begin(); iterator != vec.end(); ++iterator) {
    static std::map<int, std::function<int()>> s_map = {
        {1, [&]() { return *iterator; }},
        {2, [&]() { return *iterator; }},
        {3, [&]() { return *iterator; }},
        {4, [&]() { return *iterator; }},
        {5, [&]() { return *iterator; }},
    };
    //BOOST_TEST(s_map[*iterator]() == *iterator);
  }
}

BOOST_AUTO_TEST_CASE(test_map_install){
  const int SIZE = 100000; // 测试数据量

  // 生成随机键值对
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, SIZE);

  std::unordered_map<int, int> unorderedMap;
  std::map<int, int> orderedMap;

  // 测试插入
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    int key = dis(gen);
    int value = dis(gen);
    unorderedMap[key] = value;
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Time taken by unordered_map insertion: " << elapsed_seconds.count() << "s\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    int key = dis(gen);
    int value = dis(gen);
    orderedMap[key] = value;
  }
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  std::cout << "Time taken by map insertion: " << elapsed_seconds.count() << "s\n";

  // 测试查询
  start = std::chrono::steady_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    int key = dis(gen);
    auto it = unorderedMap.find(key);
    if (it != unorderedMap.end()) {
      // Found
    }
  }
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  std::cout << "Time taken by unordered_map find: " << elapsed_seconds.count() << "s\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < SIZE; ++i) {
    int key = dis(gen);
    auto it = orderedMap.find(key);
    if (it != orderedMap.end()) {
      // Found
    }
  }
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  std::cout << "Time taken by map find: " << elapsed_seconds.count() << "s\n";


}

BOOST_AUTO_TEST_SUITE_END()
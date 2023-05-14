//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_LINKLIST_H
#define THINKSYSTEM_LINKLIST_H
#include "DefType.h"

namespace think
{
/* 链接列表容器
 *  是vector类型容器的扩展
 * 定义了链接列表与权重之间的运算
 *  operator + - * /
 * 定义了去重
 *  void deduplicate()
 * 定义按照特殊值移除元素的方法
 *  void erase(link_val standardValue)
 * */
class LinkList : public std::vector<link_t>
{
public:
  using data_type      = link_t;
  using super          = std::vector<data_type>;

  using iterator       = super::iterator;
  using const_iterator = super::const_iterator;
  using pointer        = super::pointer;
  using reference      = super::reference;

public:
  LinkList &operator+=(int val);

  LinkList &operator-=(int val);

  LinkList &operator*=(const WEIGHT<link_val> &weight);

  LinkList operator*(const WEIGHT<link_val> &weight);

  LinkList operator-(int val);

  LinkList operator+(int val);

  void erase(link_val standardValue);

  /// @brief 去重
  /// 将相同的元素合并，链接值相加
  void deduplicate();
};// class LinkList

}// namespace think

#endif//THINKSYSTEM_LINKLIST_H

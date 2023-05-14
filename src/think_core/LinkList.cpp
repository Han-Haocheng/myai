//
// Created by HanHaocheng on 2023/05/12.
//

#include "LinkList.h"

namespace think
{
void LinkList::deduplicate()
{
  std::unordered_set<link_t, LinkHash> s;
  s.reserve(this->size());
  for (link_t &lstLink: *this)
  {
    auto [setLink, bIsAdd] = s.emplace(lstLink);
    if (!bIsAdd) { const_cast<link_val &>(setLink->linkVal) += lstLink.linkVal; }
  }
  this->assign(s.begin(), s.end());
}
void LinkList::erase(link_val standardValue)
{
  if (this->empty()) { return; }
  auto condition = [&](const link_t &item) -> bool { return item.linkVal < standardValue; };
  auto res       = std::remove_if(this->begin(), this->end(), condition);
  super::erase(res);
}

LinkList LinkList::operator+(int val)
{
  LinkList res = *this;
  return res += val;
}
LinkList LinkList::operator-(int val)
{
  LinkList res = *this;
  return res -= val;
}
LinkList LinkList::operator*(const WEIGHT<link_val> &weight)
{
  LinkList res = *this;
  return res *= weight;
}
LinkList &LinkList::operator*=(const WEIGHT<link_val> &weight)
{
  for (link_t &lk: *this) { lk.linkVal = weight * lk.linkVal; }
  return *this;
}
LinkList &LinkList::operator-=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::max<int>(NUM_LINK_VAL_MIN, lk.linkVal - val); }
  return *this;
}
LinkList &LinkList::operator+=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::min<int>(NUM_LINK_VAL_MAX, lk.linkVal + val); }
  return *this;
}
}// namespace think
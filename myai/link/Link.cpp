//
// Created by HanHaocheng on 2024/2/19.
//
#include "Link.h"

MYAI_BEGIN

Link::Link(id_t link_id, weight_t weight) : id(link_id), weight(weight) {}
Link::Link(const std::pair<id_t, weight_t> &pair) : id(pair.first), weight(pair.second) {}

MYAI_END
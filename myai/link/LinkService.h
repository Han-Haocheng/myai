//
// Created by HanHaocheng on 2024/3/22.
//

#ifndef MY_AI_LINKSERVICE_H
#define MY_AI_LINKSERVICE_H

#include "../config/macro.h"
#include "./Link.h"
#include <vector>

MYAI_BEGIN
class LinkService {
public:

  bool addLink(Link&&,std::vector<Link>&);
  bool delLinkById(id_t,std::vector<Link>&);
  bool delLinkByWeight(weight_t weight,std::vector<Link>&);
};
MYAI_END
#endif//MY_AI_LINKSERVICE_H

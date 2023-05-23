//
// Created by HanHaocheng on 2023/05/14.
//

#ifndef THINKSYSTEM_IOVIDEO_H
#define THINKSYSTEM_IOVIDEO_H

#include "IOInterface.h"
namespace think::io
{

class IOVideo final : public IOInterface
{
public:
  IOVideo() : IOInterface(ENodeType::CONST_VIDEO, (/*2048 * 2048 **/ 4), 6) {}
  ~IOVideo() final = default;

  const std::unique_ptr<LinkList>& inputActInfo() final { return this->m_lpListInputLink_; }

  void loadActFunc(std::unordered_map<node_id, ActFunc>& OutputAct) final {}
};

}// namespace think::io

#endif//THINKSYSTEM_IOVIDEO_H

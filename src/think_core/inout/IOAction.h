//
// Created by HanHaocheng on 2023/05/16.
//

#ifndef THINKSYSTEM_IOACTION_H
#define THINKSYSTEM_IOACTION_H

#include "IOInterface.h"
namespace think::io
{

class IOAction : public IOInterface
{
public:
  IOAction() : IOInterface(ENodeType::CONST_ACTION, 0xf, 0xf) {  }
  const std::shared_ptr<LinkList>& inputActInfo() final{}


  void loadActFunc(std::unordered_map<node_id, ActFunc>& OutputAct) final{}
};

}// namespace think::io

#endif//THINKSYSTEM_IOACTION_H

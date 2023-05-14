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
  IOVideo() : IOInterface(ENodeType::CONST_VIDEO) { loadActFunc(); }
  ~IOVideo() final = default;
  void getInputList(LinkList &out) override {}

private:
  void loadActFunc() override { OutputAct.emplace() }
};

}// namespace think::io

#endif//THINKSYSTEM_IOVIDEO_H

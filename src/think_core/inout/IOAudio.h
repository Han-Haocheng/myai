//
// Created by HanHaocheng on 2023/05/16.
//

#ifndef THINKSYSTEM_IOAUDIO_H
#define THINKSYSTEM_IOAUDIO_H

#include "IOInterface.h"
namespace think::io
{
class IOAudio : public IOInterface
{
public:
  IOAudio() noexcept : IOInterface(ENodeType::CONST_AUDIO, 2000, 0) {}

public:
  const std::unique_ptr<LinkList>& inputActInfo() final { return this->m_lpListInputLink_; }

  void loadActFunc(std::unordered_map<node_id, ActFunc>& OutputAct) final {}
};

}// namespace think::io

#endif//THINKSYSTEM_IOAUDIO_H

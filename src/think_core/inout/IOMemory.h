//
// Created by HanHaocheng on 2023/05/10.
//

#ifndef THINKSYSTEM_IOMEMORY_H
#define THINKSYSTEM_IOMEMORY_H
#include "IOInterface.h"

namespace think::io
{

class IOMemory final : public IOInterface
{
public:
  const static unsigned int NUM_INPUT = 4, NUM_OUTPUT = 4;
  constexpr const static IO_INFO MEMORY_INFO = IO_INFO{12, 12};

  IOMemory() : IOInterface(ENodeType::CONST_MEMORY) {}
  ~IOMemory() final { this->saveConstId(); }
  void setActStaticNode(LinkList &stLinks) {}
  void setActDynamicNode(LinkList &dyLinks) {}

  void getInputList(LinkList &out) final
  {
    // TODO 需要定义思维的直接感知，例如对于喜怒哀惧等情绪的感知
    out.emplace_back(m_lstInputId_[0], static_cast<link_val>(interferenceFactorTable.emotionActive * NUM_LINK_VAL_MAX));
  }

  void loadActFunc() final
  {
    OutputAct.emplace(m_lstOutputId_[0],
                      [](active_t actVal) -> void
                      { interferenceFactorTable.emotionActive += actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[1],
                      [](active_t actVal) -> void
                      { interferenceFactorTable.emotionActive -= actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[2],
                      [](active_t actVal) -> void
                      { interferenceFactorTable.mentalConcentration += actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[3],
                      [](active_t actVal) -> void
                      { interferenceFactorTable.mentalConcentration -= actVal / (NUM_LINK_VAL_MAX + 1); });
  }

private:
};

}// namespace think::io
#endif//THINKSYSTEM_IOMEMORY_H

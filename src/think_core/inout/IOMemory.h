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

  IOMemory() : IOInterface(ENodeType::CONST_MEMORY, 4, 4) {}
  ~IOMemory() final { this->saveConstId(); }
  void setActStaticNode(LinkList &stLinks) {}
  void setActDynamicNode(LinkList &dyLinks) {}

  const std::shared_ptr<LinkList> &inputActInfo() final
  {
    m_lpListInputLink_->emplace_back(m_lstInputId_[0], static_cast<link_val>(interference.emotionActive * NUM_LINK_VAL_MAX));
  }

  void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) final
  {
    OutputAct.emplace(m_lstOutputId_[0],
                      [](active_t actVal) -> void
                      { interference.emotionActive += actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[1],
                      [](active_t actVal) -> void
                      { interference.emotionActive -= actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[2],
                      [](active_t actVal) -> void
                      { interference.mentalConcentration += actVal / (NUM_LINK_VAL_MAX + 1); });
    OutputAct.emplace(m_lstOutputId_[3],
                      [](active_t actVal) -> void
                      { interference.mentalConcentration -= actVal / (NUM_LINK_VAL_MAX + 1); });
  }

private:
};

}// namespace think::io
#endif//THINKSYSTEM_IOMEMORY_H

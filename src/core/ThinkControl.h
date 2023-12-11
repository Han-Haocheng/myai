//
// Created by HanHaocheng on 2023/11/30.
//

#ifndef THINK_THINKCONTROL_H
#define THINK_THINKCONTROL_H

#include "NodeFactory.h"
#include "Noder.h"
#include <unordered_map>
namespace myai
{

class ThinkControl
{
public:
  void init();
  void run();

  bool stop();
  bool timeout();

private:
  NodeIdAllocator::ptr m_idAlloc = nullptr;
  NodeFactory::ptr m_nodeFactory = nullptr;
  NodeDao::ptr m_nodeDao = nullptr;
  size_t m_recordeNums = 0ULL;
  std::forward_list<Node::ptr> m_recordes = {};
  static std::unordered_map<Noder::Type, Noder::ptr> s_noderMap;
};

}// namespace myai

#endif//THINK_THINKCONTROL_H

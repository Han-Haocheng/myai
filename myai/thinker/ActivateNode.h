//
// Created by HanHaocheng on 2024/3/11.
//

#ifndef MY_AI_ACTIVATENODE_H
#define MY_AI_ACTIVATENODE_H

#include "../config/macro.h"
#include "../node/Node.h"
MYAI_BEGIN
class ActivateNode {
public:

  ActivateNode(Node::ptr node);


private:
  Node::ptr m_node;
  weight_t m_weight = 0;

};
MYAI_END
#endif//MY_AI_ACTIVATENODE_H

#ifndef MYAI_MYAI_H_
#define MYAI_MYAI_H_


#include "node.h"

MYAI_BEGIN

/// <summary>
/// src controller
/// </summary>
class MyaiController {
public:
  MyaiController();
  ~MyaiController();
  
  void run();
  void stop();
  bool is_stop();

private:
  bool m_is_stop;
  NodeFactory::ptr m_configer;
  NodeManager::ptr m_manager;
};


MYAI_END

#endif// !MYAI_MYAI_H_

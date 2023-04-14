//
// Created by HanHaocheng on 2023/04/12.
//

#ifndef THINKSYSTEM_TYPE_EXCEPT_H
#define THINKSYSTEM_TYPE_EXCEPT_H

#include <string>

class Except {
  unsigned int m_link_{};

  std::string m_except_info_;
  enum class Level{
    ERROR,
    WARING,
    INFO,
  };
public:
  Except(unsigned int line,const std::string&){

  }

  ~Except(){

  }
};


#endif//THINKSYSTEM_TYPE_EXCEPT_H

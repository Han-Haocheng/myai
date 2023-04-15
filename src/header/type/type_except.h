//
// Created by HanHaocheng on 2023/04/12.
//

#ifndef THINKSYSTEM_TYPE_EXCEPT_H
#define THINKSYSTEM_TYPE_EXCEPT_H

#include <iostream>
#include <string>

class Log : public std::exception {
  unsigned int m_link_{};

  std::string m_info_;
  enum class Level {
    ERROR,
    WARING,
    INFO,
  };

public:
  Log(Level errLevel, unsigned int line, const std::string &file, const std::string &what) {
    switch (errLevel) {

      case Level::ERROR:
        std::cout << "[错误]" << what << "\t[位置]" << file << ":" << line << std::endl;
        break;
      case Level::WARING:
        std::cout << "[警告]" << what << "\t[位置]" << file << ":" << line << std::endl;
        break;
      case Level::INFO:
        std::cout << "[信息]" << what << "\t[位置]" << file << ":" << line << std::endl;
        break;
    }
  }
};



#endif//THINKSYSTEM_TYPE_EXCEPT_H

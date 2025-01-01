//
// Created by HanHaocheng on 2024/9/5.
//

#include "myai.h"
MYAI_BEGIN

MyaiController::MyaiController()
    : m_is_stop(true),
      m_manager(m_configer->manager()) {
}

MyaiController::~MyaiController() {
}
void MyaiController::run() {
}
void MyaiController::stop() {
}
bool MyaiController::is_stop() {
  return false;
}

MYAI_END

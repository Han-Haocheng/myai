//
// Created by HanHaocheng on 2024/3/11.
//

#include "Activator.h"

MYAI_BEGIN

bool Activator::loadInfos(const Thinker::ptr &thinker) {
  if (m_nodeIds.empty()) {
    return false;
  }
  if (!m_buffer.empty()) {
    m_buffer.clear();
  }
  thinker->getNode(m_buffer, m_nodeIds.begin(), m_nodeIds.end());
  if (m_nodeIds.size() != m_nodeNum) {
    return false;
  }
  return true;
}


MYAI_END
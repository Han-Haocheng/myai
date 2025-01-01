#include "MyaiFileIO.h"

MYAI_BEGIN

void MyaiFileIO::open(nodeid_t id) {
  if (id == Node::NULL_ID) {
    MYLIB_THROW("ID为空");
  }

  String path = analysis_to_path(id);

  m_fs.open(path);
  if (!m_fs.is_open()) {
    MYLIB_THROW("文件打开失败");
  }
  m_fs.read(reinterpret_cast<byte_t *>(&m_head), sizeof(FileHead));
}

void MyaiFileIO::close() {
  if (!m_fs.is_open()) {
    return;
  }
  m_fs.close();
  m_fs.clear();
}

Node::ptr MyaiFileIO::readNode() {
  if (m_current_loc >= m_head.index_begin + m_head.index_size * sizeof(FileIndexType)) {
    MYLIB_THROW("结尾");
  }



  m_current_loc = m_fs.tellg();
  return Node::ptr();
}
MYAI_END
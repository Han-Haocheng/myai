#pragma once
#include "../define.h"
#include "../entity/Node.h"
#include <fstream>

MYAI_BEGIN

class MyaiFileIO {
public:
  using ptr = std::shared_ptr<MyaiFileIO>;
  using FileIndex = std::map<nodeid_t, size_t>;
  using FileIndexType = FileIndex::value_type;

  struct FileHead {
    const char magic_head[8] = "MYAIDBF";
    uint32 file_vision;
    size_t head_size;

    size_t index_begin;
    size_t index_size;

    size_t data_begin;
    size_t data_size;
  };

  inline const FileIndex &index() const { return m_index; }
  inline const FileHead &head() const { return m_head; }
  inline bool is_open() { return m_fs.is_open(); }

  void open(nodeid_t id);
  void close();

  Node::ptr readNode();
  Node::ptr readNode(size_t loc);
  bool writeNode(Node::ptr);
  bool writeNode(Node::ptr, size_t loc);

private:
  std::string analysis_to_path(nodeid_t id);

private:
  nodeid_t id;
  size_t m_current_loc;
  FileHead m_head;
  FileIndex m_index;
  std::fstream m_fs;
};

MYAI_END

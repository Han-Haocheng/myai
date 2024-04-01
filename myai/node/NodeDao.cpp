//
// Created by HanHaocheng on 2024/2/19.
//

#include "NodeDao.h"

#include <csignal>
#include <fstream>

MYAI_BEGIN
NodeDao::NodeDao(std::string dataRootPath) : m_rootPath(std::move(dataRootPath)) {}

int NodeDao::insert(const Node::ptr &node, bool isForced) {
  if (!node || node->id() == 0)
    return 0;

  std::string path = path_parse(node->id());
  if (!isForced && !access(path.c_str(), F_OK))
    return 0;

  std::ofstream ofs{path, std::ios::out | std::ios::binary};
  if (!ofs.is_open())
    return 0;

  ofs << node->id() << (uint32_t &) node->type() << node->bias() << node->links().size();
  for (const auto &item: node->links())
    ofs << (const uint64_t &) item;
  ofs.close();
  return 1;
}

// NodeDao::update 函数用于更新节点数据
int NodeDao::update(const Node::ptr &node) {
  // 检查节点是否为空或节点ID是否为0
  if (!node || node->id() == 0)
    return 0;

  // 解析节点ID得到节点路径
  std::string path = path_parse(node->id());

  // 检查节点路径是否存在且可写
  if (access(path.c_str(), F_OK | W_OK))
    return 0;

  // 打开节点路径，以二进制模式和追加模式打开文件
  std::ofstream ofs{path, std::ios::out | std::ios::binary | std::ios::app};
  if (!ofs.is_open())
    return 0;

  // 将节点数据写入文件
  ofs << node->id() << (uint32_t &) node->type() << node->bias() << node->links().size();
  for (const auto &item: node->links())
    ofs << (const uint64_t &) item;

  // 关闭文件
  ofs.close();

  return 1;
}

Node::ptr NodeDao::selectById(id_t id) {
  if (id == 0) {
    return nullptr;
  }
  std::string path = path_parse(id);
  std::ifstream ifs(path, std::ios::binary);
  if (!ifs.is_open()) {
    return nullptr;
  }

  Node::ptr node{new Node{}};
  size_t size;
  ifs >> node->id() >> (uint32_t &) node->type() >> node->bias() >> size;
  node->links().resize(size);
  for (auto &link: node->links())
    ifs >> (uint64_t &) link;
  ifs.close();
  return node;
}

int NodeDao::deleteById(id_t id) {
  if (id == 0) {
    return 0;
  }
  std::string path = path_parse(id);
  return unlink(path.c_str()) ? 0 : 1;
}

std::string NodeDao::path_parse(id_t id) {
  return std::string{m_rootPath + "/" +
                     std::to_string(id & 0xff) + "/" +
                     std::to_string((id >> 8) & 0xff) + "/" +
                     std::to_string((id >> 16) & 0xff) + "/" +
                     std::to_string((id >> 24) & 0xff) + ".node"};
}

MYAI_END

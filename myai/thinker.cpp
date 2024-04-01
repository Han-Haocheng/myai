
#include "thinker.h"
#include "../mylib/log/log.h"
#include <fstream>

namespace myai {

EmotionalActivator::emotion EmotionalActivator::s_emotion{};
::mylib::Config<ThinkConfig>::ptr NodeControl::s_think_conf = nullptr;
::mylib::log::Logger::ptr g_logger = nullptr;

//=====================================================================================================================
Link::Link(id_t link_id, weight_t weight) : id(link_id), weight(weight) {}
Link &Link::operator=(const char *bytes) {
  // ?��memcpy�����??��
  memcpy((void *) this, bytes, sizeof(*this));
  return *this;
}
Link::operator const char *() const { return reinterpret_cast<const char *>(this); }
Link::operator bool() const { return weight != 0 || !!(uint64_t) id; }
bool Link::operator==(const Link &rhs) const { return id == rhs.id; }
bool Link::operator!=(const Link &rhs) const { return !(rhs == *this); }
bool Link::operator<(const Link &rhs) const { return id < rhs.id; }
bool Link::operator>(const Link &rhs) const { return rhs < *this; }
bool Link::operator<=(const Link &rhs) const { return !(rhs < *this); }
bool Link::operator>=(const Link &rhs) const { return !(*this < rhs); }

//=====================================================================================================================

void LinkGroup::addLink(const Link &link) { m_datas.emplace_back(link); }
const std::vector<Link> &LinkGroup::getAllLinks() const { return m_datas; }
void LinkGroup::for_each(std::function<void(Link &)> func) { std::for_each(m_datas.begin(), m_datas.end(), std::move(func)); }
void LinkGroup::for_each(std::function<void(const Link &)> func) const { std::for_each(m_datas.begin(), m_datas.end(), std::move(func)); }

//=====================================================================================================================
Node::Node(id_t id, Type type, weight_t bias) : m_id(id), m_bias(bias), m_type(type) {}
id_t Node::getId() const { return m_id; }
weight_t Node::getBias() const { return m_bias; }
void Node::setBias(weight_t bias) { m_bias = bias; }
Node::Type Node::getType() const { return m_type; }
void Node::setType(Type type) { m_type = type; }
std::string Node::toString(Node::Type val) {
  // ?���?���?��
  switch (val) {
#define XX(ty)  \
  case NT_##ty: \
    return #ty
    XX(UNKNOWN);
    XX(MEMORY);
    XX(EMOTION);
  }
#undef XX
  return "UNKNOWN";
}

std::unordered_map<std::string, Node::Type> s_nodeTy_str_map{
    {"1", Node::NT_MEMORY},
    {"Memory", Node::NT_MEMORY},
    {"memory", Node::NT_MEMORY},
    {"MEMORY", Node::NT_MEMORY},
    {"NT_MEMORY", Node::NT_MEMORY},
};
Node::Type Node::fromString(const std::string &type) {
  auto res = s_nodeTy_str_map.find(type);
  return res != s_nodeTy_str_map.end() ? res->second : NT_UNKNOWN;
}

bool Node::operator==(const Node &rhs) const { return m_id == rhs.m_id; }
bool Node::operator!=(const Node &rhs) const { return !(rhs == *this); }
bool Node::operator<(const Node &rhs) const { return m_id < rhs.m_id; }
bool Node::operator>(const Node &rhs) const { return rhs < *this; }
bool Node::operator<=(const Node &rhs) const { return !(rhs < *this); }
bool Node::operator>=(const Node &rhs) const { return !(*this < rhs); }
LinkGroup &Node::linkGroup() { return m_linkGroup; }
const LinkGroup &Node::linkGroup() const { return m_linkGroup; }

//=====================================================================================================================
NodeDao::NodeDao(std::string dataRootPath) : m_rootPath(std::move(dataRootPath)) {}

int NodeDao::insert(const Node::ptr &node, bool isForced) {
  //�??���?��?����
  if (!node || node->m_id == 0)
    return 0;

  // ?��id��?���
  std::string path = path_parse(node->m_id);
  // �?�?�?���
  if (!isForced && !access(path.c_str(), F_OK))
    return 0;

  // ��?����
  std::ofstream ofs{path, std::ios::out | std::ios::binary};
  if (!ofs.is_open())
    return 0;

  ofs << node->m_id << (uint32_t &) node->m_type << node->m_bias << node->m_linkGroup.m_datas.size();
  for (const auto &item: node->m_linkGroup.m_datas)
    ofs << (const uint64_t &) item;
  ofs.close();
  return 1;
}

// NodeDao::update 函数用于更新节点数据
int NodeDao::update(const Node::ptr &node) {
  // 检查节点是否为空或节点ID是否为0
  if (!node || node->m_id == 0)
    return 0;

  // 解析节点ID得到节点路径
  std::string path = path_parse(node->m_id);

  // 检查节点路径是否存在且可写
  if (access(path.c_str(), F_OK | W_OK))
    return 0;

  // 打开节点路径，以二进制模式和追加模式打开文件
  std::ofstream ofs{path, std::ios::out | std::ios::binary | std::ios::app};
  if (!ofs.is_open())
    return 0;

  // 将节点数据写入文件
  ofs << node->m_id << (uint32_t &) node->m_type << node->m_bias << node->m_linkGroup.m_datas.size();
  for (const auto &item: node->m_linkGroup.m_datas)
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
  ifs >> node->m_id >> (uint32_t &) node->m_type >> node->m_bias >> size;
  node->m_linkGroup.m_datas.resize(size);
  for (auto &link: node->m_linkGroup.m_datas)
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
//=====================================================================================================================
NodeIdAllocator::NodeIdAllocator(std::string file) : m_file(std::move(file)) { load_from_file(); }
NodeIdAllocator::~NodeIdAllocator() { save_to_file(); }
id_t NodeIdAllocator::allocate() {
  if (m_ids.empty()) {
    return ++m_currentId;
  }

  id_t res{m_ids.front()};
  m_ids.pop_front();
  return res;
}
void NodeIdAllocator::deallocate(id_t id) { m_ids.emplace_front(id); }
void NodeIdAllocator::load_from_file() {}
void NodeIdAllocator::save_to_file() {}
id_t NodeIdAllocator::allocate(size_t i) {
  id_t res = m_currentId;
  m_currentId += i;
  return res;
}

//=====================================================================================================================
Activator::Activator(Node::Type type) : m_type(type) {}
//=====================================================================================================================
MemoryActivator::MemoryActivator(NodeDao::ptr nodeDao) : Activator(Node::NT_MEMORY), m_memoryLinks(new LinkGroup{}), m_nodeDao(std::move(nodeDao)) {}

void MemoryActivator::getActivateInfos(std::unordered_map<id_t, weight_t> &link_info) {
  m_memoryLinks->for_each([&](Link &link) {
    auto fd_rt = link_info.find(link.id);
    if (fd_rt == link_info.end()) {
      Node::ptr temp_node = m_nodeDao->selectById(link.id);
      fd_rt = link_info.emplace(link.id, temp_node->getBias()).first;
    }
    fd_rt->second += link.weight;
  });
}
void MemoryActivator::active(const std::unordered_map<id_t, ActiveNode> &actives) {
  for (auto &aNodePair: actives) {
    auto node = aNodePair.second.getNode();
    node->linkGroup().for_each([&, this](const Link &link) {
      m_memoryLinks->addLink(link.id, link.weight * link.weight);
    });
  }
}

//=====================================================================================================================

EmotionalActivator::EmotionalActivator(id_t id_begin) : Activator(Node::NT_EMOTION), m_begin(id_begin) {}

void EmotionalActivator::getActivateInfos(std::unordered_map<id_t, weight_t> &link_info) {
  s_emotion.emotion_interference > 1.0
      ? link_info[m_begin + IN_POSITIVE] = (s_emotion.emotion_interference - 1.0f) * 10000.0f
      : link_info[m_begin + IN_NEGATIVE] = s_emotion.emotion_interference * 10000.0f;
  s_emotion.activates_standard > 10000.0f
      ? link_info[m_begin + IN_CONCENTRATION] = s_emotion.activates_standard
      : link_info[m_begin + IN_DISPERSION] = 10000.0f - s_emotion.activates_standard;
}

void EmotionalActivator::active(const std::unordered_map<id_t, ActiveNode> &actives) {
  auto findRes = actives.find(m_begin + OUT_POSITIVE);
  if (findRes != actives.end()) {
    s_emotion.activates_standard += findRes->second.weight() / 10000.0f;
  }
  findRes = actives.find(m_begin + OUT_NEGATIVE);
  if (findRes != actives.end()) {
    s_emotion.activates_standard -= findRes->second.weight() / 10000.0f;
  }
  findRes = actives.find(m_begin + OUT_CONCENTRATION);
  if (findRes != actives.end()) {
    s_emotion.activates_standard = std::max(s_emotion.activates_standard + findRes->second.weight(), 20000.0f);
  }
  findRes = actives.find(m_begin + OUT_DISPERSION);
  if (findRes != actives.end()) {
    s_emotion.activates_standard = std::max(s_emotion.activates_standard - findRes->second.weight(), 0.0f);
  }
}

//=====================================================================================================================
Thinker::Thinker(NodeDao::ptr dao, NodeIdAllocator::ptr idAlloc) : m_idAlloc(std::move(idAlloc)), m_nodeDao(std::move(dao)) {}

void Thinker::activate(const Node::ptr &per_node, const Node::ptr &next_node) {
  std::unordered_map<id_t, weight_t> linkInfo;
  for (const auto &active_node: m_activeCache) {
    active_node.second.getActivator()->getActivateInfos(linkInfo);
    // active_node.second.activator->getActivateInfos(linkInfo);
  }

  std::vector<Node::ptr> activeNodes;
  for (const auto &link: linkInfo) {
    per_node->linkGroup().addLink(link.first, link.second);

    Node::ptr node = m_nodeDao->selectById(link.first);
    if (!node) {
      continue;
    }

    node->linkGroup().addLink(next_node->getId(), link.second);
    activeNodes.emplace_back(node);
    // m_activtors[node->getType()]->active(Link{link});
  }
}

Node::ptr Thinker::addRecordNode(weight_t bias) {
  Node::ptr node = std::make_shared<Node>(m_idAlloc->allocate(), Node::NT_MEMORY, bias);
  if (m_nodeDao->insert(node, false) == 0) {
    m_idAlloc->deallocate(node->getId());
    return nullptr;
  }
  return node;
}
id_t Thinker::addNodes(Node::Type type, size_t size, std::vector<Node::ptr> &out_nodes) {
  out_nodes.clear();
  out_nodes.reserve(size);
  id_t begin = m_idAlloc->allocate(size);
  for (size_t i = 0; i < size; ++i) {
    out_nodes.emplace_back(std::make_shared<Node>(begin + i, type, 0));
  }
  return begin;
}
void Thinker::addActivator(const Activator::ptr &activator) { m_activtors.emplace_back(activator); }
void Thinker::clearActivator() { m_activtors.clear(); }
Node::ptr Thinker::getNode(id_t id) {
  auto fd_rt = m_nodeCache.find(id);
  if (fd_rt != m_nodeCache.end() || fd_rt->second->m_isDelete) {
    return fd_rt->second;
  }
  auto sl_rt = m_nodeDao->selectById(id);
  if (sl_rt) {
    m_nodeCache.emplace(id, sl_rt);
  }
  return sl_rt;
}
bool Thinker::delNode(id_t id) {
  auto node = getNode(id);
  if (!node) {
    return false;
  }
  node->m_isDelete = true;
  return false;
}
void Thinker::clear_cache() {
  for (const auto &node_pair: m_nodeCache) {
    if (node_pair.second->m_isDelete) {
      continue;
    }
    m_nodeDao->insert(node_pair.second, false);
  }
}

//=====================================================================================================================

void NodeControl::init() {
  if (!g_logger) {
    g_logger = MYLIB_LOGGER_NAME("think");
    g_logger->setLevel(MYLIB_LL_DEBUG);
    g_logger->setFormatter(std::make_shared<::mylib::log::LogFormatter>("%d{%H:%M:%S} %t%T%F%T[%c - %p][%f:%l]%m%n"));
    g_logger->addAppender(std::make_shared<::mylib::log::ConsoleAppender>(MYLIB_LL_DEBUG));
  }
  if (!s_think_conf) {
    s_think_conf = ::mylib::Configer::GetInstance()->setConfig<ThinkConfig>("think", {
                                                                                         {0, {}},
                                                                                         {
                                                                                             {Node::Type::NT_MEMORY, 0, 0},
                                                                                             {Node::Type::NT_EMOTION, 1, 8},
                                                                                         },
                                                                                         10000,
                                                                                     },
                                                                            "think");
  }
  auto &config = s_think_conf->getValue();
  m_idAlloc.reset(new NodeIdAllocator(config.idAllocator.currentId, config.idAllocator.ids.begin(), config.idAllocator.ids.end()));
  m_nodeDao.reset(new NodeDao());
  m_noder.reset(new Thinker(m_nodeDao, m_idAlloc));
  m_maxRecordeNum = config.maxRecordeNum;

  m_records = {};
  m_recordeNum = 0ULL;

  auto &noders_config = config.noders;

  auto conf_it = noders_config.begin();
  static std::unordered_map<Node::Type, std::function<Activator::ptr()>> s_type_activator_map = {
      {Node::Type::NT_MEMORY, [this]() { return std::make_shared<MemoryActivator>(m_nodeDao); }},
      {Node::Type::NT_EMOTION, [&]() { return std::make_shared<EmotionalActivator>(conf_it->idStart); }},
  };

  for (; conf_it != noders_config.end(); ++conf_it) {
    m_noder->addActivator(s_type_activator_map[conf_it->type]());
  }
}

void NodeControl::run() {
  for (; !isStop();) {
    inferencePeriod();
    learnPeriod();
  }
}

void NodeControl::inferencePeriod() {
  Node::ptr per_node = nullptr, next_node = nullptr;
  for (; m_recordeNum < m_maxRecordeNum; ++m_recordeNum) {
    if ((m_recordeNum + 1) % 100 == 0) {
      MYLIB_LOG_INFO(g_logger) << "run " << m_recordeNum + 1 << " times";
    }
    next_node = m_noder->addRecordNode(5);
    m_records.emplace_front(next_node);
    ;
    m_noder->activate(per_node, next_node);
    per_node = next_node;
    next_node = nullptr;
  }
}
void NodeControl::learnPeriod() {

  for (auto pre_iter = m_records.before_begin(),
            iter = m_records.begin();
       iter != m_records.end();
       pre_iter = iter, ++iter) {
    //    (**pre_iter).linkGroup().for_each([](const Link &link) {
    //
    //    });
    m_nodeDao->insert(*iter, false);
  }
  m_recordeNum = 0;
}
bool NodeControl::isStop() {
  return false;
}
NodeControl::ptr NodeControl::getInstance() {
  static NodeControl::ptr instance;
  if (!instance) {
    instance.reset(new NodeControl());
  }

  return instance;
}
NodeControl::NodeControl() { init(); }

}// namespace myai
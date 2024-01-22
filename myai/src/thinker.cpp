
#include "thinker.h"

namespace myai {
EmotionalActivator::emotion EmotionalActivator::s_emotion{};
::mylib::Config<ThinkConfig>::ptr NodeControl::s_think_conf = nullptr;

//=====================================================================================================================
NodeIdAllocator::NodeIdAllocator(std::string file) : m_file(std::move(file)) { load_from_file(); }
NodeIdAllocator::~NodeIdAllocator() { save_to_file(); }
id_t NodeIdAllocator::allocate()
{
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
id_t NodeIdAllocator::allocate(size_t i)
{
  id_t res = m_currentId;
  m_currentId += i;
  return res;
}
NodeIdAllocator::NodeIdAllocator(id_t begin, const std::forward_list<id_t> &ids) : m_currentId(begin), m_ids(ids)
{
}

//=====================================================================================================================
NodeDao::NodeDao(std::string dataRootPath)
    : m_rootPath(std::move(dataRootPath))
{
}

int NodeDao::insert(const Node::ptr &node, bool isForced)
{
  if (!node || node->m_id == 0)
    return 0;

  std::string path = path_parse(node->m_id);
  /*if (!isForced && ! access(path.c_str(), F_OK))*/
  return 0;

  std::ofstream ofs{path, std::ios::out | std::ios::binary};
  if (!ofs.is_open())
    return 0;

  ofs << node->m_id << (uint32_t &) node->m_type << node->m_bias << node->m_linkGroup.m_linkNum;
  for (const auto &item: node->m_linkGroup.m_datas)
    ofs << (const uint64_t &) item;
  ofs.close();
  return 1;
}
int NodeDao::update(const Node::ptr &node)
{
  if (!node || node->m_id == 0)
    return 0;

  std::string path = path_parse(node->m_id);

  /*if (access(path.c_str(), F_OK | W_OK))*/
  return 0;

  std::ofstream ofs{path, std::ios::out | std::ios::binary | std::ios::trunc};
  if (!ofs.is_open())
    return 0;

  ofs << node->m_id << (uint32_t &) node->m_type << node->m_bias << node->m_linkGroup.m_linkNum;
  for (const auto &item: node->m_linkGroup.m_datas)
    ofs << (const uint64_t &) item;
  ofs.close();
  return 1;
}

int NodeDao::deleteById(id_t id)
{
  if (id == 0) {
    return 0;
  }
  std::string path = path_parse(id);

#if __linux__
  return unlink(path.c_str()) ? 0 : 1;

#elif _WIN32
  return _unlink(path.c_str()) ? 0 : 1;

#endif
}

Node::ptr NodeDao::selectById(id_t id)
{
  if (id == 0) {
    return nullptr;
  }
  std::string path = path_parse(id);
  std::ifstream ifs(path, std::ios::binary);
  if (!ifs.is_open()) {
    return nullptr;
  }

  Node::ptr node{new Node{}};
  ifs >> node->m_id >> (uint32_t &) node->m_type >> node->m_bias >> node->m_linkGroup.m_linkNum;
  for (size_t i = 0; i < node->m_linkGroup.m_linkNum; ++i) {
    Link tmpLink;
    ifs >> (uint64_t &) tmpLink;
    node->m_linkGroup.m_datas.emplace_front(tmpLink);
  }
  ifs.close();
  return node;
}

std::string NodeDao::path_parse(id_t id)
{
  return std::string{m_rootPath + "/" +
                     std::to_string(id & 0xff) + "/" +
                     std::to_string((id >> 8) & 0xff) + "/" +
                     std::to_string((id >> 16) & 0xff) + "/" +
                     std::to_string((id >> 24) & 0xff) + ".node"};
}

//=====================================================================================================================
Activator::Activator(Node::Type type) : m_type(type) {}

//=====================================================================================================================
Noder::Noder(NodeDao::ptr dao, NodeIdAllocator::ptr idAlloc) : m_idAlloc(std::move(idAlloc)), m_nodeDao(std::move(dao)) {}

void Noder::activate(const Node::ptr &per_node, const Node::ptr &next_node)
{
  std::unordered_map<id_t, weight_t> linkInfo;
  for (const auto &active_node: m_activeCache) {
    active_node.second.activator->setActivationInfo(linkInfo);
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
    m_activtors[node->getType()]->active(Link{link});
  }
}

Node::ptr Noder::addRecordNode(weight_t bias)
{
  Node::ptr node = std::make_shared<Node>(m_idAlloc->allocate(), Node::NT_MEMORY, bias);
  if (m_nodeDao->insert(node, false) == 0) {
    m_idAlloc->deallocate(node->getId());
    return nullptr;
  }
  return node;
}
id_t Noder::addNodes(Node::Type type, size_t size, std::vector<Node::ptr> &out_nodes)
{
  out_nodes.clear();
  out_nodes.reserve(size);
  id_t begin = m_idAlloc->allocate(size);
  for (size_t i = 0; i < size; ++i) {
    out_nodes.emplace_back(std::make_shared<Node>(begin + i, type, 0));
  }
  return begin;
}
void Noder::addActivator(const Activator::ptr &activator) { m_activtors.emplace_back(activator); }
void Noder::clearActivator() { m_activtors.clear(); }
Node::ptr Noder::getNode(id_t id)
{
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
bool Noder::delNode(id_t id)
{
  auto node = getNode(id);
  if (!node) {
    return false;
  }
  node->m_isDelete = true;
  return false;
}
void Noder::clear_cache()
{
  for (const auto &node_pair: m_nodeCache) {
    if (node_pair.second->m_isDelete) {
      continue;
    }
    m_nodeDao->insert(node_pair.second, false);
  }
}

//=====================================================================================================================

void NodeControl::init()
{
  if (!g_logger) {
    g_logger = MYLIB_LOGGER_NAME("think");
    g_logger->setLevel(::mylib::LogEvent::LL_DEBUG);
    g_logger->setFormatter(std::make_shared<::mylib::LogFormatter>("%d{%H:%M:%S} %t%T%F%T[%c - %p][%f:%l]%m%n"));
    g_logger->addAppender(std::make_shared<::mylib::ConsoleAppender>(::mylib::LogEvent::LL_DEBUG));
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
  m_idAlloc.reset(new NodeIdAllocator(config.idAllocator.currentId, config.idAllocator.ids));
  m_nodeDao.reset(new NodeDao());
  m_noder.reset(new Noder(m_nodeDao, m_idAlloc));
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

void NodeControl::run()
{
  for (; !isStop();) {
    inferencePeriod();
    learnPeriod();
  }
}
void NodeControl::inferencePeriod()
{
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
void NodeControl::learnPeriod()
{

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
  ;
}
bool NodeControl::isStop()
{

  return false;
}
NodeControl::ptr NodeControl::getInstance()
{
  static NodeControl::ptr instance;
  if (!instance) {
    instance.reset(new NodeControl());
  }

  return instance;
}
NodeControl::NodeControl()
{
  init();
}
//=====================================================================================================================
MemoryActivator::MemoryActivator(NodeDao::ptr nodeDao) : Activator(Node::NT_MEMORY), m_memoryLinks(new LinkGroup{}), m_nodeDao(std::move(nodeDao))
{
}
void MemoryActivator::setActivationInfo(std::unordered_map<id_t, weight_t> &link_info)
{
  m_memoryLinks->for_each([&](Link &link) {
    auto fd_rt = link_info.find(link.id);
    if (fd_rt == link_info.end()) {
      Node::ptr temp_node = m_nodeDao->selectById(link.id);
      fd_rt = link_info.emplace(link.id, temp_node->getBias()).first;
    }
    fd_rt->second += link.weight;
  });
}
void MemoryActivator::active(const Link &link)
{
  const auto node = m_nodeDao->selectById(link.id);
  node->linkGroup().for_each([&, this](const Link &link) {
    m_memoryLinks->addLink(link.id, link.weight * link.weight);
  });
}

//=====================================================================================================================

EmotionalActivator::EmotionalActivator(id_t id_begin)
    : Activator(Node::NT_EMOTION), m_begin(id_begin)
{
}
void EmotionalActivator::setActivationInfo(std::unordered_map<id_t, weight_t> &link_info)
{
  s_emotion.emotion_interference > 1.0
      ? link_info[m_begin + IN_POSITIVE] = (s_emotion.emotion_interference - 1.0f) * 10000.0f
      : link_info[m_begin + IN_NEGATIVE] = s_emotion.emotion_interference * 10000.0f;
  s_emotion.activates_standard > 10000.0f
      ? link_info[m_begin + IN_CONCENTRATION] = s_emotion.activates_standard
      : link_info[m_begin + IN_DISPERSION] = 10000.0f - s_emotion.activates_standard;
}
void EmotionalActivator::active(const Link &link)
{
  auto emotionIo = (EmotionIO) (link.id - m_begin);
  switch (emotionIo) {
    case OUT_POSITIVE:
      s_emotion.activates_standard += link.weight / 10000.0f;
      break;
    case OUT_NEGATIVE:
      s_emotion.activates_standard -= link.weight / 10000.0f;
      break;
    case OUT_CONCENTRATION:
      s_emotion.activates_standard = std::max<>(s_emotion.activates_standard + link.weight, 20000.0f);
      break;
    case OUT_DISPERSION:
      s_emotion.activates_standard = std::max<>(s_emotion.activates_standard - link.weight, 0.0f);
      break;
    default:
      // error
      break;
  }
}
}
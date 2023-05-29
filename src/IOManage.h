//
// Created by HanHaocheng on 2023/05/16.
//

#ifndef THINKSYSTEM_IOMANAGE_H
#define THINKSYSTEM_IOMANAGE_H
#include "ThinkCore.h"

namespace think
{
#pragma pack(push, 2)
namespace io
{
  class IOInterface
  {
  public:
    struct InterferenceFactorTable
    {
      weight_t<active_t> decayFrequency      = 0.01;
      weight_t<active_t> emotionActive       = {1.0, 5.0, 0.2};//情绪活跃权重
      weight_t<link_val> mentalConcentration = {1.0, 5.0, 0.2};//精神集中权重
    };
    const struct const_node_info
    {
      size_t in_size  = 0;
      size_t out_size = 0;
    } io_size;

  public:
    constexpr static const char *STR_CONST_ID_FOLDER = ".\\const_id\\";
    static InterferenceFactorTable interference;//干扰系数表
  protected:
    const ENodeType m_eIoType_;

    std::vector<node_id> m_lstOutputId_;
    LinkList m_lstInputLink_;

    //TODO 需要定义一个天生条件反射类型，用于将输入直接转为相应输出

  public:
    explicit IOInterface(ENodeType nType, size_t inSize, size_t outSize)
    : m_eIoType_(nType),
      io_size({inSize, outSize}),
      m_lstInputLink_(inSize),
      m_lstOutputId_(outSize)
    {
    }

    virtual const LinkList &inputActInfo()                                    = 0;
    virtual void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) = 0;

    bool saveConstId()
    {
      fs::path cIdPath = std::string(STR_CONST_ID_FOLDER) + ARRAY_TYPE_TO_STRING[(int) m_eIoType_] + ".dat";
      if (!fs::exists(cIdPath.parent_path())) { fs::create_directories(cIdPath.parent_path()); }
      std::ofstream idFile{cIdPath.string(), std::ios::binary | std::ios::out};

      if (!idFile.is_open())
      {
        std::cerr << "产生未知错误，静态节点id文件写入时打开失败！" << std::endl;
        return false;
      }

      idFile.write(reinterpret_cast<const char *>(&io_size), sizeof(const_node_info));
      for (const auto &[id, val]: m_lstInputLink_)
      {
        idFile.write(reinterpret_cast<const char *>(&id), sizeof(node_id));
      }
      for (const node_id &id: m_lstOutputId_) { idFile.write(reinterpret_cast<const char *>(&id), sizeof(node_id)); }
      idFile.close();
      return true;
    }

    void initConstId(NodeIdAllocator &IdAlloc, std::unordered_map<node_id, std::shared_ptr<NodeInfo>> &NodeBuffer)
    {
      std::string csPath = std::string(STR_CONST_ID_FOLDER) + ARRAY_TYPE_TO_STRING[(int) m_eIoType_] + ".dat";

      std::ifstream csFile;
      std::ios_base::iostate exceptionMask = csFile.exceptions() | std::ios::failbit;

      csFile.exceptions(exceptionMask);
      try
      {
        csFile.open(csPath, std::ios::binary | std::ios::in);
      }
      catch (std::ios_base::failure &e)
      {
        if (!fs::exists(csPath))
        {
          std::cerr << "文件不存在，静态节点id文件读取时打开失败！" << std::endl;

          for (auto &[id, val]: m_lstInputLink_)
          {
            id  = IdAlloc.allocate();
            val = 0;
          }
          IdAlloc.allocate(m_lstOutputId_, io_size.out_size);
          std::cerr << "节点已经分配。" << std::endl;
          return;
        }
        std::cerr << e.what() << std::endl;
        return;
      }

      csFile.read(reinterpret_cast<char *>(const_cast<const_node_info *>(&io_size)), sizeof(const_node_info));

      for (auto &[id, val]: m_lstInputLink_)
      {
        csFile.read(reinterpret_cast<char *>(&id), sizeof(node_id));
        auto *tmp = new NodeInfo(id);
        NodeBuffer.emplace(id, tmp);
        val = 0;
      }

      m_lstOutputId_ = std::vector<node_id>(io_size.out_size);
      for (node_id &id: m_lstOutputId_) { csFile.read(reinterpret_cast<char *>(&id), sizeof(node_id)); }

      csFile.close();
    }
  };//class IOInterface

  IOInterface::InterferenceFactorTable IOInterface::interference = {};

  class IOMemory final : public IOInterface
  {
  public:
    const static unsigned int NUM_INPUT = 4, NUM_OUTPUT = 4;

    IOMemory() : IOInterface(ENodeType::CONST_MEMORY, NUM_INPUT, NUM_OUTPUT) {}

    const LinkList &inputActInfo() final
    {
      weight_t<link_val> emo = interference.emotionActive.val() - 1.0;
      weight_t<link_val> men = interference.mentalConcentration.val() - 1.0;
      if (emo.val() > 0)
      {
        m_lstInputLink_[0].linkVal = emo * 0x800;
        interference.emotionActive.ref() -= interference.decayFrequency.val();
      }
      else
      {
        m_lstInputLink_[1].linkVal = (-emo) * 0x800;
        interference.emotionActive.ref() += interference.decayFrequency.val();
      }
      if (men.val() > 0)
      {
        m_lstInputLink_[2].linkVal = men * 0x800;
        interference.mentalConcentration.ref() -= interference.decayFrequency.val();
      }
      else
      {
        m_lstInputLink_[3].linkVal = (-men) * 0x800;
        interference.mentalConcentration.ref() += interference.decayFrequency.val();
      }
      return m_lstInputLink_;
    }

    void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) final
    {
      OutputAct.emplace(this->m_lstOutputId_[0],
                        [](active_t aVal) -> void
                        { interference.emotionActive += interference.decayFrequency * aVal; });
      OutputAct.emplace(this->m_lstOutputId_[1],
                        [](active_t aVal) -> void
                        { interference.emotionActive -= interference.decayFrequency * aVal; });
      OutputAct.emplace(this->m_lstOutputId_[2],
                        [](active_t aVal) -> void
                        { interference.mentalConcentration += interference.decayFrequency * aVal; });
      OutputAct.emplace(this->m_lstOutputId_[3],
                        [](active_t aVal) -> void
                        { interference.mentalConcentration -= interference.decayFrequency * aVal; });
    }

  private:
  };

  class IOVideo final : public IOInterface
  {
  public:
    IOVideo() : IOInterface(ENodeType::CONST_VIDEO, 2048, 6) {}

    const LinkList &inputActInfo() final { return this->m_lstInputLink_; }

    void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) final {}
  };

  class IOAudio : public IOInterface
  {
  public:
    IOAudio() noexcept : IOInterface(ENodeType::CONST_AUDIO, 1000, 0) {}

  public:
    const LinkList &inputActInfo() final { return this->m_lstInputLink_; }

    void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) final {}
  };

  class IOAction : public IOInterface
  {
  public:
    IOAction() : IOInterface(ENodeType::CONST_ACTION, 1000, 1000) {}
    const LinkList &inputActInfo() final { return this->m_lstInputLink_; }

    void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) final {}
  };

}// namespace io

/// \b 多输入输出 感知-单节点-多节点-行为 双层节点架构
class IOManage
{
private:
  NodeIdAllocator m_idAlloca;
  std::unordered_map<node_id, ActFunc> m_mapAction_;
  std::vector<std::shared_ptr<io::IOInterface>> m_lstIOS_;

  std::unordered_map<node_id, std::shared_ptr<NodeInfo>> m_nodeBuffer_;//节点缓存

  std::unique_ptr<NodeInfo> m_actedLinks_        = std::make_unique<NodeInfo>();

  constexpr static const link_val NUM_DEF_DESIRE = 0x800;
  constexpr static const link_val NUM_DEF_MEMORY = 0x800;

public:
  active_t m_focusStand_ = 0x800;

public:
  size_t getNodeBufferSize() { return m_nodeBuffer_.size(); }
  IOManage() : m_idAlloca(NUM_NODE_ID_MAX)
  {
    m_lstIOS_.emplace_back(std::make_unique<io::IOMemory>());
    m_lstIOS_.emplace_back(std::make_unique<io::IOVideo>());
    m_lstIOS_.emplace_back(std::make_unique<io::IOAudio>());
    m_lstIOS_.emplace_back(std::make_unique<io::IOAction>());
    for (const auto &item: m_lstIOS_)
    {
      item->initConstId(m_idAlloca, m_nodeBuffer_);
      item->loadActFunc(m_mapAction_);
    }
  }
  ~IOManage()
  {
    for (const auto &item: m_lstIOS_) { item->saveConstId(); }
  }

  void loadLink(std::unordered_map<node_id, std::shared_ptr<NodeInfo>> &actBuffer, const link_t &lk)
  {
    auto findRes = actBuffer.find(lk.id);
    if (findRes != actBuffer.end()) { findRes->second->actVal += lk.linkVal; }
    else
    {
      auto findRes2 = m_nodeBuffer_.find(lk.id);
      if (findRes2 != m_nodeBuffer_.end())
      {
        findRes2->second->actVal = lk.linkVal;
        actBuffer.emplace(lk.id, findRes2->second);
      }
      else
      {
        node_type &empRes     = *m_nodeBuffer_.emplace(lk.id, std::make_unique<NodeInfo>(lk.id)).first;
        empRes.second->actVal = lk.linkVal;
        actBuffer.emplace(lk.id, empRes.second);
      }
    }
  }

  size_t runLoop()
  {
    static int LoopNum = 0;
    std::cout << "runLoop - " << LoopNum << " - begin" << std::endl;
    std::unordered_map<node_id, std::shared_ptr<NodeInfo>> activeNodebuf;//激活节点缓存

    //新连接节点
    node_type &firstNode = *m_nodeBuffer_.emplace(m_idAlloca.allocate(), std::make_unique<NodeInfo>()).first;

    //
    for (const auto &item: m_lstIOS_)
    {
     auto& a = item->inputActInfo();
      for (const link_t &lk: a) {
        loadLink(activeNodebuf, lk);
      }
    }

    for (const link_t &lk: m_actedLinks_->staticLink) loadLink(activeNodebuf, lk);

    for (const link_t &lk: m_actedLinks_->dynamicLink) loadLink(activeNodebuf, lk);

    firstNode.second = std::move(m_actedLinks_);

    m_actedLinks_    = std::make_unique<NodeInfo>(ENodeType::DYNAMIC_TYPE);
    for (auto &[id, info]: activeNodebuf)
    {
      info->dynamicLink.emplace(firstNode.first, NUM_DEF_MEMORY);
      info->act(io::IOInterface::interference.emotionActive,
                io::IOInterface::interference.mentalConcentration * m_focusStand_, *m_actedLinks_);
    }
    std::cout << "runLoop - " << LoopNum++ << " - end" << std::endl;
    return activeNodebuf.size();
  }
  void stableThink()
  {
    for (const auto &[id, info]: m_nodeBuffer_)
    {
      info->stableLink(0.2);
      info->writeNodeInfo(id);
    }
    m_nodeBuffer_.clear();
    for (const auto &item: m_lstIOS_) item->initConstId(m_idAlloca, m_nodeBuffer_);
  }

private:
};//! IOManage
#pragma pack(pop)
}// namespace think

#endif//THINKSYSTEM_IOMANAGE_H

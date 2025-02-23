#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

#include "define.h"
#include "Edge.h"


MYAI_BEGIN



/**
 * @brief 用于保存节点
 */
class MyaiNode : public ISerialize {
	friend class MyaiDatabase;
	friend class MyaiService;

public:
	constexpr static const size_t MAX_LINK_NUMS = 0x1000;
	constexpr static const weight_t NULL_WEIGHT = 0.0;
	constexpr static const nodeid_t NULL_ID		= 0;

	using const_ptr								= std::shared_ptr<const MyaiNode>;
	using ptr									= std::shared_ptr<MyaiNode>;
	using enum_size								= uint32;
	enum State : enum_size {
		NDS_UNDEFINED,// 未定义
		NDS_CREATE,	  // 创建：未分配id
		NDS_FILL,	  // 填充数据：只有基础属性，没有链接属性
		NDS_READY,	  // 就绪：可以进行操作的节点
		NDS_ACTIVE,	  // 激活：已经完成激活的节点
		NDS_SYNC,	  // 整理：已经完成数据整理
		NDS_SAVE,	  // 保存：已经和硬盘同步的节点
		NDS_FREE,	  // 释放：已经释放的节点
		NDS_DESTROY,  // 销毁：id被释放
	};

	MyaiNode() = default;
	MyaiNode(nodeid_t id, weight_t bias, State state) : m_id(id), m_bias(bias), m_state(state) {}
	MyaiNode(nodeid_t id, weight_t bias, State state, EdgeList &links) : m_id(id), m_bias(bias), m_state(state), m_links(links) {}
	~MyaiNode() override = default;

	[[nodiscard]] auto bias() const { return m_bias; }
	[[nodiscard]] auto id() const { return m_id; }
	[[nodiscard]] auto state() const { return m_state; }
	[[nodiscard]] const auto &links() const { return m_links; }

	auto &links() { return m_links; }
	auto &buffer() { return m_buffer; }

	void serialize(std::ostream &out) const override;
	void deserialize(std::istream &in) override;

private:
	nodeid_t m_id;
	weight_t m_bias;
	State m_state;

	EdgeList m_links;
	EdgeList m_buffer;
};


MYAI_END

#endif// !MYAI_NODE_H_

#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

#include <array>
#include <fstream>
#include <utility>

#include "../define.h"
#include "Link.h"

MYAI_BEGIN

class ISerialize{
public:
	virtual void serialize(std::ostream& out) const = 0;
	virtual void deserialize(std::istream& in) = 0;
};

/**
 * @brief 用于保存节点
 */
class Node: public ISerialize {
	friend class MyaiDatabase;
	friend class NodeService;

public:
	constexpr static const size_t MAX_LINK_NUMS = 0x1000;
	constexpr static const weight_t NULL_WEIGHT = 0.0;
	constexpr static const nodeid_t NULL_ID = 0;

	using const_ptr = std::shared_ptr<const Node>;
	using ptr = std::shared_ptr<Node>;

	enum State
	{	
		NDS_UNDEFINED,// 0 未定义
		NDS_CREATE,// 1 创建：未分配id
		NDS_FILL,// 2 填充数据：只有基础属性，没有链接属性
		NDS_READY,// 3 就绪：可以进行操作的节点
		NDS_ACTIVE,// 4 激活：已经完成激活的节点
		NDS_SYNC,// 5 整理：已经完成数据整理
		NDS_SAVE,// 6 保存：已经和硬盘同步的节点
		NDS_FREE,// 7 释放：已经释放的节点
		NDS_DESTROY,// 8 销毁：id被释放
	};

	struct Head {
		nodeid_t id;
		weight_t bias;
		State state;
	};

	Node(Head&& data) : m_head(std::move(data)),m_links(),m_buffer() {}
	Node(const Head& data) : m_head(data),m_links(),m_buffer(){}
	Node(nodeid_t id = NULL_ID, weight_t bias = NULL_WEIGHT, State state = NDS_UNDEFINED) : m_head({id, bias, state}),m_links(),m_buffer() {}
	~Node() = default;

	inline auto bias() const { return m_head.bias; }
	inline auto id() const { return m_head.id; }
	inline auto state() const { return m_head.state; }
	inline auto size() const { return m_links.size(); }
	inline const auto &data() const { return m_head; }
	inline const auto &links() const { return m_links; }
	inline const auto &buffer() { return m_buffer; }

	void serialize(std::ostream& out) const override;
	void deserialize(std::istream& in) override;
	
private:
	Head m_head;
	std::vector<Link> m_links;
	std::vector<Link> m_buffer;
};



//class NodeLinkStream {
//public:
//	using ptr = std::shared_ptr<NodeLinkStream>;
//
//	std::unordered_map<nodeid_t, weight_t> stream;
//
//	NodeLinkStream &operator<<(const std::vector<Link> &lks) {
//		for (auto &lk: lks) {
//			stream.emplace(lk.id, lk.weight);
//		}
//		return *this;
//	}
//
//	NodeLinkStream &operator>>(std::vector<Link> &lks) {
//		for (auto &lk: stream) {
//			lks.emplace(lk.first, lk.second);
//		}
//		return *this;
//	}
//
//	[[nodiscard]] std::vector<Link> &&links() const {
//		std::vector<Link> res;
//		res.reserve(stream.size());
//		for (auto &lk: stream) {
//			res.emplace(lk);
//		}
//		return std::move(res);
//	}
//};

MYAI_END

#endif// !MYAI_NODE_H_

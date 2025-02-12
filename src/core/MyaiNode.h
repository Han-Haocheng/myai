#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

<<<<<<< HEAD
#include "Edge.h"
#include "define.h"
#include <functional>

=======
#include "define.h"
#include "Edge.h"

<<<<<<< HEAD
#include <array>
#include <fstream>
#include <unordered_map>
#include <utility>
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)

MYAI_BEGIN


<<<<<<< HEAD
/**
<<<<<<< HEAD
=======
 * @brief 用于保存链接
 */
union Edge {
	constexpr static const nodeid_t NULL_ID		= 0ULL;
	constexpr static const weight_t NULL_WEIGHT = 0.0;
	using pair_t								= std::pair<nodeid_t, weight_t>;
	struct {
		nodeid_t id;
		weight_t weight;
	};

	pair_t pair;

	explicit Edge(const nodeid_t i = NULL_ID, const weight_t w = NULL_WEIGHT) : id(i), weight(w) {}
	explicit Edge(const std::pair<const nodeid_t, weight_t> &p) : pair(p) {}
	Edge(Edge &&)	   = default;
	Edge(const Edge &) = default;
	~Edge()			   = default;

	Edge &operator=(Edge &&rhs) noexcept;
	Edge &operator=(const Edge &rhs);
};


class EdgeList {
public:
	using ptr									 = std::shared_ptr<EdgeList>;
	using iterator								 = std::vector<Edge>::iterator;
	using const_iterator						 = std::vector<Edge>::const_iterator;

	EdgeList()									 = default;
	~EdgeList()									 = default;
	EdgeList(EdgeList &&)						 = default;
	EdgeList(const EdgeList &)					 = default;
	EdgeList &operator=(EdgeList &&rhs) noexcept = default;
	EdgeList &operator=(const EdgeList &rhs)	 = default;

	Edge &emplace(const nodeid_t &key, const weight_t &mapped);

	iterator find(const nodeid_t &key) {
		auto fd_rt = m_map.find(key);
		if (fd_rt == m_map.end()) {
			return m_links.end();
		}
		return iterator(&fd_rt->second);
	}

	iterator begin() { return m_links.begin(); }
	iterator end() { return m_links.end(); }
	const_iterator begin() const { return m_links.begin(); }
	const_iterator end() const { return m_links.end(); }
	size_t size() const { return m_links.size(); }
	bool empty() const { return m_links.empty(); }

private:
	std::vector<Edge> m_links;
	std::unordered_map<nodeid_t, Edge &> m_map;
};
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)

/**
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
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

<<<<<<< HEAD
	void for_each(const std::function<void(Edge &)> &cb) {
		for (auto &link: m_links) {
			cb(link.second);
		}
		for (auto &link: m_buffer) {
			cb(link.second);
		}
	}

=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
private:
	nodeid_t m_id;
	weight_t m_bias;
	State m_state;

	EdgeList m_links;
	EdgeList m_buffer;
};


<<<<<<< HEAD
<<<<<<< HEAD
=======
//class NodeLinkStream {
//public:
//	using ptr = std::shared_ptr<NodeLinkStream>;
//
//	std::unordered_map<nodeid_t, weight_t> stream;
//
//	NodeLinkStream &operator<<(const std::vector<Edge> &lks) {
//		for (auto &lk: lks) {
//			stream.emplace(lk.id, lk.weight);
//		}
//		return *this;
//	}
//
//	NodeLinkStream &operator>>(std::vector<Edge> &lks) {
//		for (auto &lk: stream) {
//			lks.emplace(lk.first, lk.second);
//		}
//		return *this;
//	}
//
//	[[nodiscard]] std::vector<Edge> &&links() const {
//		std::vector<Edge> res;
//		res.reserve(stream.size());
//		for (auto &lk: stream) {
//			res.emplace(lk);
//		}
//		return std::move(res);
//	}
//};

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
MYAI_END

#endif// !MYAI_NODE_H_

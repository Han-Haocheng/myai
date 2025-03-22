#ifndef MYAI_NODE_H_
#define MYAI_NODE_H_

#include "Edge.h"
#include <functional>
#include <utility>


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
	constexpr static const nodeid_t NULL_ID = 0;

	using const_ptr = std::shared_ptr<const MyaiNode>;
	using ptr = std::shared_ptr<MyaiNode>;
	using enum_size = uint32;

	enum State : enum_size {
		NDS_UNDEFINED,
		NDS_CREATE,
		NDS_FILL,
		NDS_READY,
		NDS_ACTIVE,
		NDS_SYNC,
		NDS_SAVE,
		NDS_FREE,
		NDS_DESTROY,
	};

	MyaiNode() = default;
	MyaiNode(nodeid_t id, weight_t bias, State state) : m_id(id), m_bias(bias), m_state(state) {}
	MyaiNode(nodeid_t id, weight_t bias, State state, EdgeList links) : m_id(id), m_bias(bias), m_state(state), m_links(std::move(links)) {}
	~MyaiNode() override = default;

	[[nodiscard]] auto bias() const { return m_bias; }
	[[nodiscard]] auto id() const { return m_id; }
	[[nodiscard]] auto state() const { return m_state; }
	[[nodiscard]] const auto &links() const { return m_links; }

	auto &links() { return m_links; }
	auto &buffer() { return m_buffer; }

	void serialize(std::ostream &out) const override;
	void deserialize(std::istream &in) override;

	void for_each(const std::function<void(Edge &)> &cb) {
		for (auto &[_, link]: m_links) { cb(link); }
		for (auto &[id , link]: m_buffer) { cb(link); }
	}

private:
	nodeid_t m_id;
	weight_t m_bias;
	State m_state;

	EdgeList m_links;
	EdgeList m_buffer;
};


MYAI_END

#endif// !MYAI_NODE_H_
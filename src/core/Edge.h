
#ifndef MYAI_EDGE_H_
#define MYAI_EDGE_H_

#include "define.h"
#include <unordered_map>

MYAI_BEGIN

/**
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

// using EdgeList = std::unordered_map<nodeid_t, Edge>;


class EdgeList {
public:
	using ptr									 = std::shared_ptr<EdgeList>;
	using value_type							 = Edge;
	using container								 = std::unordered_map<nodeid_t, value_type>;
	using iterator								 = container::iterator;
	using const_iterator						 = container::const_iterator;
	using reference								 = value_type &;
	using const_reference						 = const value_type &;

	EdgeList()									 = default;
	~EdgeList()									 = default;
	EdgeList(EdgeList &&)						 = default;
	EdgeList(const EdgeList &)					 = default;
	EdgeList &operator=(EdgeList &&rhs) noexcept = default;
	EdgeList &operator=(const EdgeList &rhs)	 = default;


	iterator begin() { return m_map.begin(); }
	iterator end() { return m_map.end(); }
	const_iterator begin() const { return m_map.begin(); }
	const_iterator end() const { return m_map.end(); }
	size_t size() const { return m_map.size(); }
	bool empty() const { return m_map.empty(); }

	Edge &emplace(const value_type &key);
	Edge &emplace(nodeid_t id, weight_t weight) { return emplace(value_type{id, weight}); }
	iterator find(const nodeid_t &key);
	void insert(EdgeList::const_iterator first, EdgeList::const_iterator last);
	void insert(const EdgeList &list);

private:
	container m_map;
};


MYAI_END

#endif// !MYAI_EDGE_H_

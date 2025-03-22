#ifndef MYAI_EDGE_H_
#define MYAI_EDGE_H_

#include "../MyaiDefine.h"

#include <unordered_map>

MYAI_BEGIN

/**
 * @brief 用于保存链接
 */
struct Edge {


	nodeid_t id;
	weight_t weight;

	explicit Edge(nodeid_t i = NULL_ID, weight_t w = NULL_WEIGHT);
	Edge(Edge &&)						 = default;
	Edge(const Edge &)					 = default;
	~Edge()								 = default;
	Edge &operator=(Edge &&rhs) noexcept = default;
	Edge &operator=(const Edge &rhs)	 = default;
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


	inline iterator begin() { return m_map.begin(); }
	inline iterator end() { return m_map.end(); }
	inline const_iterator begin() const { return m_map.begin(); }
	inline const_iterator end() const { return m_map.end(); }
	inline size_t size() const { return m_map.size(); }
	inline bool empty() const { return m_map.empty(); }

	iterator find(const nodeid_t &key);
	
	Edge &emplace(const value_type &key);
	Edge &emplace(nodeid_t id, weight_t weight) { return emplace(value_type{id, weight}); }
	void insert(EdgeList::const_iterator first, EdgeList::const_iterator last);
	void insert(const EdgeList &list);
	void insert(EdgeList::ptr list) { insert(list->begin(), list->end()); }

private:
	container m_map;
};


MYAI_END

#endif// !MYAI_EDGE_H_

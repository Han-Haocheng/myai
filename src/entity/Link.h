#ifndef MYLIB_ENTITY_LINK_H
#define MYLIB_ENTITY_LINK_H

#include "../define.h"
#include <functional>
#include <map>


MYAI_BEGIN

/**
 * @brief 用于保存链接
 */
union Link {
	constexpr static const nodeid_t NULL_ID		= 0ULL;
	constexpr static const weight_t NULL_WEIGHT = 0.0;
	using pair_t								= std::pair<nodeid_t, weight_t>;
	struct {
		nodeid_t id;
		weight_t weight;
	};

	pair_t pair;

	explicit Link(const nodeid_t i = NULL_ID, const weight_t w = NULL_WEIGHT) : id(i), weight(w) {}
	explicit Link(const std::pair<const nodeid_t, weight_t> &p) : pair(p) {}
	Link(Link &&)	   = default;
	Link(const Link &) = default;
	~Link()			   = default;

	Link &operator=(Link &&rhs) noexcept;
	Link &operator=(const Link &rhs);
};


class LinkList : std::map<nodeid_t, weight_t> {
	using super = std::map<nodeid_t, weight_t>;

public:
	using ptr = std::shared_ptr<LinkList>;
	using std::map<nodeid_t, weight_t>::begin;
	using std::map<nodeid_t, weight_t>::end;
	using std::map<nodeid_t, weight_t>::size;

	LinkList()									 = default;
	~LinkList()									 = default;
	LinkList(LinkList &&)						 = default;
	LinkList(const LinkList &)					 = default;
	LinkList &operator=(LinkList &&rhs) noexcept = default;
	LinkList &operator=(const LinkList &rhs)	 = default;

	LinkList &operator<<(LinkList &&ll) {
		merge(std::move(ll));
		return *this;
	}

	LinkList &operator<<(const LinkList &ll) {
		this->insert(ll.begin(), ll.end());
		return *this;
	}

	size_t eraseByLessStand(weight_t w);

	void downWeight(weight_t w);
	void upWeight(weight_t w);
	weight_t averageWeight() const;


	iterator emplace(const nodeid_t &key, const weight_t &mapped);


	//void insert(LinkList &&ll);
	//void insert(const LinkList &ll);


private:
};

MYAI_END

#endif// !MYLIB_ENTITY_LINK_H

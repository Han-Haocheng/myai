#pragma once
#include "../entity/Link.h"

MYAI_BEGIN
class LinkStream {
public:
	using ptr = std::shared_ptr<LinkStream>;

private:
	std::vector<Link> m_links;
};
MYAI_END
#ifndef MYAI_DAO_MYAIDAO_H
#define MYAI_DAO_MYAIDAO_H

#include "MyaiFileIO.h"

MYAI_BEGIN

class MyaiDao {
public:
	using ptr = std::shared_ptr<MyaiDao>;

	int insert(Node::ptr node);
	int updata(Node::ptr node);
	int deleteById(nodeid_t id);
	Node::ptr selectById(nodeid_t id);

private:
	bool check_node_legality(Node::ptr node);
	String aniseya_path(nodeid_t id);
private:
	MyaiFileIO::ptr m_file_io;
	
};

MYAI_END

#endif //MYAI_DAO_MYAIDAO_H

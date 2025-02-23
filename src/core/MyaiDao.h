#ifndef MYAI_DAO_MYAIDAO_H
#define MYAI_DAO_MYAIDAO_H

#include "MyaiFileIO.h"

MYAI_BEGIN

class MyaiDao {
public:
	using ptr = std::shared_ptr<MyaiDao>;

	int insert(MyaiNode::ptr node);
	int updata(MyaiNode::ptr node);
	int deleteById(nodeid_t id);
	MyaiNode::ptr selectById(nodeid_t id);

private:
	bool check_node_legality(MyaiNode::ptr node);
	String aniseya_path(nodeid_t id){
		return "";
	}

private:
	MyaiFileIO::ptr m_file_io;
};

MYAI_END

#endif//MYAI_DAO_MYAIDAO_H

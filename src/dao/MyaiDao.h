#ifndef MYAI_DAO_MYAIDAO_H
#define MYAI_DAO_MYAIDAO_H

#include <utility>

#include "MyaiFileIO.h"

MYAI_BEGIN


class MyaiDao {
public:
	using ptr = std::shared_ptr<MyaiDao>;

	explicit MyaiDao(String data_path)
		: m_data_path(std::move(data_path)),
		  m_file_io(std::make_shared<MyaiFileIO>()) {}
	int insert(MyaiNode::ptr node);
	int update(MyaiNode::ptr node);
	int deleteById(nodeid_t id);
	MyaiNode::ptr selectById(nodeid_t id);

private:
	String analyze_path(nodeid_t id) const {
		return m_data_path + "/" + std::to_string(id) + ".node";
	}

private:
	String m_data_path;
	MyaiFileIO::ptr m_file_io;
};

MYAI_END

#endif//MYAI_DAO_MYAIDAO_H

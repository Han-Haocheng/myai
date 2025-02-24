#ifndef MYAI_DAO_MYAIDAO_H
#define MYAI_DAO_MYAIDAO_H

#include "MyaiFileIO.h"

MYAI_BEGIN

class MyaiDao {
public:
	using ptr = std::shared_ptr<MyaiDao>;

<<<<<<< HEAD
<<<<<<< HEAD
	MyaiDao(String data_path)
		: m_data_path(data_path),
		  m_file_io(std::make_shared<MyaiFileIO>()) {}
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	MyaiDao(String data_path)
		: m_data_path(data_path),
		  m_file_io(std::make_shared<MyaiFileIO>()) {}
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	int insert(MyaiNode::ptr node);
	int updata(MyaiNode::ptr node);
	int deleteById(nodeid_t id);
	MyaiNode::ptr selectById(nodeid_t id);

private:
<<<<<<< HEAD
<<<<<<< HEAD
	String analyze_path(nodeid_t id) {
		return m_data_path + "/" + std::to_string(id) + ".node";
	}

private:
	String m_data_path;
=======
	bool check_node_legality(MyaiNode::ptr node);
	String aniseya_path(nodeid_t id){
		return "";
	}

private:
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	String analyze_path(nodeid_t id) {
		return m_data_path + "/" + std::to_string(id) + ".node";
	}

private:
	String m_data_path;
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	MyaiFileIO::ptr m_file_io;
};

MYAI_END

#endif//MYAI_DAO_MYAIDAO_H

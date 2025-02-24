#include "MyaiDao.h"

MYAI_BEGIN

int MyaiDao::insert(MyaiNode::ptr node) {
	if (!node || !node->id() == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

<<<<<<< HEAD
<<<<<<< HEAD
	String path = analyze_path(node->id());
=======
	String path = aniseya_path(node->id());
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	String path = analyze_path(node->id());
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::updata(MyaiNode::ptr node) {
	if (!node || !node->id() == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

<<<<<<< HEAD
<<<<<<< HEAD
	String path = analyze_path(node->id());
=======
	String path = aniseya_path(node->id());
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	String path = analyze_path(node->id());
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::deleteById(nodeid_t id) {
	if (id == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

<<<<<<< HEAD
<<<<<<< HEAD
	String path = analyze_path(id);
	m_file_io->open(path);

=======
	String path = aniseya_path(id);
	m_file_io->open(path);
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	String path = analyze_path(id);
	m_file_io->open(path);

>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	return m_file_io->eraseId(id);
}

MyaiNode::ptr MyaiDao::selectById(nodeid_t id) {
	if (id == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

<<<<<<< HEAD
<<<<<<< HEAD
	String path = analyze_path(id);
	m_file_io->open(path);

	MyaiNode::ptr res = std::make_shared<MyaiNode>();
	if (m_file_io->read(res)) {
=======
	String path = aniseya_path(id);
	m_file_io->open(path);

	MyaiNode::ptr res = std::make_shared<MyaiNode>();
	if (m_file_io->read(res)){
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	String path = analyze_path(id);
	m_file_io->open(path);

	MyaiNode::ptr res = std::make_shared<MyaiNode>();
	if (m_file_io->read(res)) {
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
		return res;
	}
	return nullptr;
}

MYAI_END
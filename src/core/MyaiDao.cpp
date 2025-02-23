#include "MyaiDao.h"

MYAI_BEGIN

int MyaiDao::insert(MyaiNode::ptr node) {
	if (!node || !node->id() == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

	String path = aniseya_path(node->id());
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::updata(MyaiNode::ptr node) {
	if (!node || !node->id() == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

	String path = aniseya_path(node->id());
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::deleteById(nodeid_t id) {
	if (id == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

	String path = aniseya_path(id);
	m_file_io->open(path);
	return m_file_io->eraseId(id);
}

MyaiNode::ptr MyaiDao::selectById(nodeid_t id) {
	if (id == MyaiNode::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

	String path = aniseya_path(id);
	m_file_io->open(path);

	MyaiNode::ptr res = std::make_shared<MyaiNode>();
	if (m_file_io->read(res)){
		return res;
	}
	return nullptr;
}

MYAI_END
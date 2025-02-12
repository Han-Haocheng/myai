#include "MyaiDao.h"

MYAI_BEGIN

int MyaiDao::insert(Node::ptr node) {
	if (!node || !node->id() == Node::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

	String path = aniseya_path(node->id());
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::updata(Node::ptr node) {
	if (!node || !node->id() == Node::NULL_ID) {
		MYLIB_THROW("avg error: node is null or id is null");
	}

	String path = aniseya_path(node->id());
	m_file_io->open(path);

	m_file_io->write(node);
	return 0;
}

int MyaiDao::deleteById(nodeid_t id) {
	if (id == Node::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

	String path = aniseya_path(id);
	m_file_io->open(path);
	return m_file_io->eraseId(id);
}

Node::ptr MyaiDao::selectById(nodeid_t id) {
	if (id == Node::NULL_ID) {
		MYLIB_THROW("avg error:  id is null");
	}

	String path = aniseya_path(id);
	m_file_io->open(path);

	Node::ptr res = std::make_shared<Node>();
	if (m_file_io->read(res)){
		return res;
	}
	return nullptr;
}

MYAI_END
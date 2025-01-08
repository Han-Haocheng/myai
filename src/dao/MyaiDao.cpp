#include "MyaiDao.h"

MYAI_BEGIN

int MyaiDao::insert(Node::ptr node) {
	m_file_io->write(node);
	return 0;
}

int MyaiDao::updata(Node::ptr node) {
	m_file_io->write(node);
	return 0;
}

int MyaiDao::deleteById(nodeid_t id) {
	return 0;
}

Node::ptr MyaiDao::selectById(nodeid_t id) {
	Node::ptr res = std::make_shared<Node>(id);
	m_file_io->read(res);
	return res;
}

MYAI_END
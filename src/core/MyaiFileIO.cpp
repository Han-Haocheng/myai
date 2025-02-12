#include "MyaiFileIO.h"

#ifdef MYLIB_WINDOWS
#include <windows.h>
#elif MYLIB_LINUX
<<<<<<< HEAD
#include <limits.h>
#include <stdlib.h>

=======
#include <stdlib.h>
#include <limits.h>
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
#endif

MYAI_BEGIN

<<<<<<< HEAD
MyaiFileIO::MyaiFileIO(size_t node_max_num)
	: m_node_max_num(node_max_num) {
}

=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
void MyaiFileIO::open(std::string path) {
	// check path
	if (check_path_is_equal(path)) return;

	// open init
	m_current_path = path;
<<<<<<< HEAD

	if (m_fs.is_open()) close();
	m_fs.open(m_current_path, std::ios::in | std::ios::out | std::ios::binary);
	if (!m_fs.is_open()) MYLIB_THROW("file error: file open failed.");

=======
	
	if (m_fs.is_open()) close();
	m_fs.open(m_current_path, std::ios::in | std::ios::out | std::ios::binary);
	if (!m_fs.is_open()) MYLIB_THROW("file error: file open failed.");
	
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
	// read init
	read_head();
	read_index(m_head);
}

void MyaiFileIO::close() {
	if (!m_fs.is_open()) {
		return;
	}

	write_index(m_head);
	write_head();

	m_fs.close();
	m_fs.clear();
}

bool MyaiFileIO::read(MyaiNode::ptr node) {
	if (!m_fs.is_open()) MYLIB_THROW("file error:file is not open");
	if (!node) MYLIB_THROW("avg error:avg is nullptr");

	auto g_rt = get_node_pos(node->id());

	if (g_rt == MyaiNode::NULL_ID) return false;
	read_node(node, g_rt);
	return true;
}

bool MyaiFileIO::write(const MyaiNode::ptr &node) {
	if (!m_fs.is_open()) MYLIB_THROW("file error:file is not open");
	if (!node) MYLIB_THROW("avg error:avg is nullptr");

	const auto g_rt = get_node_pos(node->id());

	if (g_rt == MyaiNode::NULL_ID) {
		MYLIB_ASSERT(m_index.size() < m_head.max_node_num, "avg error: index is full");
		m_fs.seekp(m_head.index_offset + m_head.max_node_num * m_head.head_size);
		m_index.emplace(node->id(), m_fs.tellp());
	}

	write_node(node, g_rt);
	return true;
}

std::streampos MyaiFileIO::get_node_pos(nodeid_t id) const noexcept {
	auto fd_rt = m_index.find(id);
	if (fd_rt == m_index.end()) {
		return MyaiNode::NULL_ID;
	}
	return fd_rt->second;
}

void MyaiFileIO::read_head() noexcept {
	m_fs.seekg(0);
	char magic_head[8];
	m_fs.read(reinterpret_cast<byte_t *>(&magic_head), sizeof(magic_head));
	if (std::string(magic_head) == MyaiFileIO::MAGIC_HEAD) {
		m_fs.read(reinterpret_cast<byte_t *>(&m_head), m_head.head_size);
	} else {
		m_head = FileHead();
	}
}

void MyaiFileIO::write_head() noexcept {
	m_fs.seekg(0);
	m_fs.write(reinterpret_cast<const byte_t *>(&MAGIC_HEAD), sizeof(MAGIC_HEAD));
	m_fs.write(reinterpret_cast<const byte_t *>(&m_head), m_head.head_size);
}

void MyaiFileIO::read_index(const FileHead &head) noexcept {
	m_fs.seekg(head.index_offset);
	FileIndex::value_type index;
	for (int i = 0; i < head.index_num; ++i) {
		m_fs.read(reinterpret_cast<byte_t *>(&m_index), head.index_size);
		m_index.emplace(index);
	}
}

void MyaiFileIO::write_index(FileHead &head) noexcept {
	head.index_num = m_index.size();
	m_fs.seekg(head.index_offset);
	for (auto &i: m_index) {
		m_fs.write(reinterpret_cast<const byte_t *>(&i), head.index_size);
	}
}

void MyaiFileIO::read_node(MyaiNode::ptr node, std::streampos pos) noexcept {
	m_fs.seekg(pos);
	node->deserialize(m_fs);
}

void MyaiFileIO::write_node(MyaiNode::ptr node, std::streampos pos) noexcept {
	m_fs.seekp(pos);
	node->serialize(m_fs);
}

bool MyaiFileIO::check_path_is_equal(String other) const noexcept {
#ifdef MYLIB_WINDOWS
	char fullpath[2][MAX_PATH];
<<<<<<< HEAD
	GetFullPathName(other.c_str(), MAX_PATH, fullpath[0], NULL);
	GetFullPathName(m_current_path.c_str(), MAX_PATH, fullpath[1], NULL);
#elif MYLIB_LINUX
	char fullpath[2][PATH_MAX];

	realpath(other.c_str(), fullpath[0]);
	realpath(m_current_path.c_str(), fullpath[1]);

	if (strcmp(fullpath1, fullpath2) == 0) {
		std::cout << "The paths are equivalent." << std::endl;
	} else {
		std::cout << "The paths are not equivalent." << std::endl;
	}
#endif// DEBUG
=======
    GetFullPathName(other.c_str(), MAX_PATH, fullpath[0], NULL);
    GetFullPathName(m_current_path.c_str(), MAX_PATH, fullpath[1], NULL);
#elif MYLIB_LINUX
	char fullpath[2][PATH_MAX];

    realpath(other.c_str(), fullpath[0]);
    realpath(m_current_path.c_str(), fullpath[1]);

    if (strcmp(fullpath1, fullpath2) == 0) {
        std::cout << "The paths are equivalent." << std::endl;
    } else {
        std::cout << "The paths are not equivalent." << std::endl;
    }
#endif // DEBUG
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
	return strcmp(fullpath[0], fullpath[1]) == 0;
}

MYAI_END
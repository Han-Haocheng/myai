#include "MyaiFileIO.h"

MYAI_BEGIN

void MyaiFileIO::open(std::string path) {
	m_fs.open(path, std::ios::in | std::ios::out | std::ios::binary);
	if (!m_fs.is_open()) {
		MYLIB_THROW("文件打开失败");
	}

	// 读取文件头
	m_fs.read(reinterpret_cast<byte_t *>(&m_head), m_head.head_size);
	if (std::string(m_head.magic_head) != MAGIC_HEAD) {
		m_head.file_vision	= FILE_VISION;
		m_head.head_size	= sizeof(FileHead);
		m_head.max_node_num = MAX_NODE_NUM;
		m_head.index_offset = sizeof(FileHead);
		m_head.index_size	= sizeof(FileIndex::value_type);
	}

	// 读取索引
	m_fs.seekg(m_head.index_offset);
	FileIndex::value_type index;
	for (int i = 0; i < m_head.index_num; ++i) {
		m_fs.read(reinterpret_cast<byte_t *>(&index), sizeof(FileIndex::value_type));
		m_index.emplace(index);
	}
}

void MyaiFileIO::close() {
	if (!m_fs.is_open()) {
		return;
	}
	m_head.index_num = m_index.size();
	m_fs.seekp(0);
	m_fs.write(reinterpret_cast<byte_t *>(&m_head), m_head.head_size);
	m_fs.seekp(m_head.index_offset);
	for (auto &index: m_index) {
		m_fs.write(reinterpret_cast<byte_t *>(&index), sizeof(FileIndex::value_type));
	}

	m_fs.close();
	m_fs.clear();
}

bool MyaiFileIO::read(Node::ptr node) {
	if (!node) {
		MYLIB_THROW("avg error:avg is nullptr");
	}

	auto g_rt = get_node_pos(node->id());
	if (g_rt == -1) {
		return false;
	}

	read_base(node, dynamic_cast<std::ifstream&>(m_fs), g_rt);
	return true;
}

bool MyaiFileIO::write(Node::ptr node) {
	if (!node) {
		MYLIB_THROW("avg error:avg is nullptr");
	}
	auto g_rt = get_node_pos(node->id());
	if (g_rt == -1) {
		if (m_index.size() >= m_head.max_node_num) {
			MYLIB_THROW("avg error:index is full");
		}
		m_fs.seekp(0, std::ios::end);
		m_index.emplace(node->id(), m_fs.tellp());
	}

	write_base(node, dynamic_cast<std::ofstream&>(m_fs), g_rt);
	return true;
}

std::streampos MyaiFileIO::get_node_pos(nodeid_t id) const noexcept {
	auto fd_rt = m_index.find(id);
	if (fd_rt == m_index.end()) {
		return -1;
	}
	return fd_rt->second;
}

void MyaiFileIO::read_base(Node::ptr node, std::ifstream &ifs, std::streampos pos) noexcept {
	ifs.seekg(pos);
	node->deserialize(ifs);
}

void MyaiFileIO::write_base(Node::ptr node, std::ofstream &ofs, std::streampos pos) noexcept {
	ofs.seekp(pos);
	node->serialize(ofs);
}

MYAI_END
#ifndef MYAI_DAO_MYAIFILEIO_H
#define MYAI_DAO_MYAIFILEIO_H

#include "../define.h"
#include "../entity/Node.h"

#include <fstream>

MYAI_BEGIN


class MyaiFileIO {
public:
	using ptr								 = std::shared_ptr<MyaiFileIO>;
	using FileIndex							 = std::map<nodeid_t, std::streampos>;
	constexpr static char MAGIC_HEAD[]		 = "MYAIDBF";
	constexpr static uint32 FILE_VISION		 = 1;
	constexpr static size_t DEF_MAX_NODE_NUM = 0x10000;

	enum FileVision {
		IOFV_UNCOMPULANT,
	};

	struct FileHead {
		uint32 file_vision	= IOFV_UNCOMPULANT;
		size_t head_size	= sizeof(FileHead);
		size_t max_node_num = DEF_MAX_NODE_NUM;
		size_t index_offset = sizeof(FileHead);
		size_t index_size	= sizeof(FileIndex::value_type);
		size_t index_num	= 0;
	};

	struct NodeHead {
		weight_t bias;
		size_t size;
		size_t begin;
	};


	MyaiFileIO(size_t node_max_num = DEF_MAX_NODE_NUM);

	inline const FileIndex &index() const { return m_index; }
	inline const FileHead &head() const { return m_head; }
	inline bool is_open() { return m_fs.is_open(); }

	void open(std::string path);
	void close();

	bool read(Node::ptr node);
	bool write(Node::ptr node);

	int eraseId(nodeid_t id){
		return m_index.erase(id);
	}

private:
	std::streampos get_node_pos(nodeid_t id) const noexcept;

	void read_head() noexcept;
	void write_head() noexcept;
	void read_index(const FileHead &head) noexcept;
	void write_index(FileHead &head) noexcept;
	void read_node(Node::ptr node, std::streampos pos) noexcept;
	void write_node(Node::ptr node, std::streampos pos) noexcept;

	bool check_path_is_equal(String other ) const noexcept;
private:
	const size_t m_node_max_num;// 最大节点数量
	String m_current_path; // 当前文件路径
	FileHead m_head; // 文件头
	FileIndex m_index; // 节点索引
	std::fstream m_fs; // 文件流
};

MYAI_END

#endif//MYAI_DAO_MYAIFILEIO_H
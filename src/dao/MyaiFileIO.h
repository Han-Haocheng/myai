#ifndef MYAI_DAO_MYAIFILEIO_H
#define MYAI_DAO_MYAIFILEIO_H

#include "../define.h"
#include "../entity/Node.h"

#include <fstream>

MYAI_BEGIN


class MyaiFileIO {
public:
	using ptr		= std::shared_ptr<MyaiFileIO>;
	using FileIndex = std::map<nodeid_t, std::streampos>;
	constexpr static char MAGIC_HEAD[] = "MYAIDBF";
	constexpr static uint32 FILE_VISION = 1;
	constexpr static size_t MAX_NODE_NUM = 0x10000;
	
	enum FileVision{
		IOFV_UNCOMPULANT,
	};

	struct FileHead {
		const char magic_head[8] = "MYAIDBF";
		uint32 file_vision		 = IOFV_UNCOMPULANT;
		size_t head_size		 = sizeof(FileHead);
		size_t max_node_num		 = MAX_NODE_NUM;
		size_t index_offset		 = sizeof(FileHead);
		size_t index_size		 = sizeof(FileIndex::value_type);
		size_t index_num;

		FileHead() = default;
		FileHead(FileHead&&) = default;
		FileHead(const FileHead&) = default;
		FileHead&operator=(FileHead&&) = default;
		FileHead&operator=(const FileHead&) = default;
		~FileHead() = default;
	};

	struct NodeHead {
		weight_t bias;
		size_t size;
		size_t begin;
	};


	inline const FileIndex &index() const { return m_index; }
	inline const FileHead &head() const { return m_head; }
	inline bool is_open() { return m_fs.is_open(); }

	void open(std::string path);
	void close();

	bool read(Node::ptr node);
	bool write(Node::ptr node);
private:
	std::streampos get_node_pos(nodeid_t id)const noexcept;
	static void read_base(Node::ptr node, std::ifstream& ifs,std::streampos pos) noexcept;
	static void write_base(Node::ptr node, std::ofstream& ofs,std::streampos pos) noexcept;
private:
	FileHead m_head;
	FileIndex m_index;
	std::fstream m_fs;
};

MYAI_END

#endif//MYAI_DAO_MYAIFILEIO_H
#ifndef MYAI_DRIVER_H_
#define MYAI_DRIVER_H_

#include "../core/Edge.h"

#include <functional>
#include <map>


MYAI_BEGIN


/**
 * @brief 驱动
 */
class MyaiDriver {
public:
	using ptr					= std::shared_ptr<MyaiDriver>;
	using ControlConnectionFunc = std::function<void(weight_t)>;
	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;

	enum Type {
		DT_MEMORY,
		DT_STATUS,
		DT_STRING,
		DT_AUDIO,
		DT_SCREEN_VIDEO,
		DT_CAMERA_VIDEO,

		__DT_END__
	};


	MyaiDriver(Type type, nodeid_t begin, size_t id_size)
		: m_type(type), m_begin(begin), m_id_size(id_size), m_collects(std::make_shared<EdgeList>()) {
	}

	virtual ~MyaiDriver() = default;

	void init() {
		regeiste_controls();
	}

	EdgeList::ptr collect() {
		collect_data();
		return m_collects;
	}

	auto getCollects() { return m_collects; }

protected:
	using super						 = MyaiDriver;


	virtual void collect_data()		 = 0;
	virtual void regeiste_controls() = 0;


protected:
	Type m_type;
	nodeid_t m_begin;
	size_t m_id_size;

	EdgeList::ptr m_collects;
};


MYAI_END


MYAI_BEGIN

//size = 1 0000H+1 0000H
class StringDriver : public MyaiDriver {
};

MYAI_END

MYAI_BEGIN

//size = 800H+800H
class AudioDriver : public MyaiDriver {
public:
private:
};

MYAI_END

MYAI_BEGIN

//size = 40 0000H+4H
class ScreenVideoDriver : public MyaiDriver {
};

//size = 40 0000H+8H
class CameraVideoDriver : public MyaiDriver {
};

MYAI_END

#endif// !MYAI_DRIVER_H_

#ifndef MYAI_DRIVER_H_
#define MYAI_DRIVER_H_

#include "../entity/Edge.h"

#include <functional>
#include <map>


MYAI_BEGIN


/**
 * @brief 驱动
 */
class MyaiDriver {
public:
	using ptr	 = std::shared_ptr<MyaiDriver>;
	using Reflex = std::function<void(weight_t)>;
	//using ConditionedReflex =

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
		set_reflexes();
	}

	auto collect(EdgeList::ptr out) {
		collect_data();
		m_collects->insert(m_collects->begin(), m_collects->end());
	}

	const std::vector<std::pair<noid_t, Reflex>> &reflexes() { return m_reflexes; }

	auto collects() { return m_collects; }
	[[nodiscard]] auto type() const { return m_type; }

protected:
	using super					= MyaiDriver;
	virtual void collect_data() = 0;
	virtual void set_reflexes() = 0;

protected:
	Type m_type;
	nodeid_t m_begin;
	size_t m_id_size;

	EdgeList::ptr m_collects;
	std::vector<std::pair<noid_t, Reflex>> m_reflexes;
};

using Driver = MyaiDriver;

MYAI_END

MYAI_BEGIN

//size = 1 0000H+1 0000H
class StringDriver : public MyaiDriver {
public:

private:
	void collect_data() override {

	}

	void set_reflexes() override {

	}

};

MYAI_END

MYAI_BEGIN

//size = 800H+800H
class AudioDriver : public MyaiDriver {
public:
private:
};

MYAI_END

#include<QtGui>
#include<QtMultimedia/QtMultimedia>

MYAI_BEGIN

//size = 40 0000H+4H
class ScreenVideoDriver : public MyaiDriver {
public:

private:
	QMediaCaptureSession m_session;
};

//size = 40 0000H+8H
class CameraVideoDriver : public MyaiDriver {
};

MYAI_END

#endif// !MYAI_DRIVER_H_

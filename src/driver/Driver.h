#ifndef MYAI_DRIVER_H_
#define MYAI_DRIVER_H_

#include "../core/MyaiNode.h"

#include <functional>
#include <map>


MYAI_BEGIN


class MyaiStatus {
public:
	using ptr	 = std::shared_ptr<MyaiStatus>;

	MyaiStatus() = default;
	void setEmotion(weight_t emotion) { m_emotion = emotion; }
	void setFocus(weight_t focus) { m_focus = focus; }

	weight_t emotion() const { return m_emotion; }
	weight_t focus() const { return m_focus; }


private:
	// 正向权重
	// 反向权重
	// 过滤权重
	// 链接权重列表
	std::vector<std::vector<weight_t>> m_driver_weights;

	weight_t m_emotion;
	weight_t m_focus;
};


/**
 * @brief 驱动
 */
class MyaiDriver {
	friend class DriverManager;

public:
	using ptr								= std::shared_ptr<MyaiDriver>;

	MYLIB_CONSTEXPR_TYPE MAX_DRIVER_NODE_ID = 0x1000'0000;

	enum Type {
		DT_STATUS,
		DT_STRING,
		DT_AUDIO,
		DT_SCREEN_VIDEO,
		DT_CAMERA_VIDEO,
	};


	MyaiDriver(Type type, nodeid_t begin, size_t id_size)
		: m_type(type), m_begin(begin), m_id_size(id_size) {
	}

	virtual ~MyaiDriver() = default;

	void init() {
		regeiste_controls();
	}

	/// @brief 收集链接
	/// @return 链接列表
	EdgeList::ptr collect() {
		collect_data();
		return m_collects;
	}


protected:
	using super						 = MyaiDriver;
	using ControlConnectionFunc		 = std::function<void(weight_t)>;

	virtual void collect_data()		 = 0;
	virtual void regeiste_controls() = 0;

	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;


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

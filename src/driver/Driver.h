#ifndef MYAI_DRIVER_H_
#define MYAI_DRIVER_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include "../core/Edge.h"
=======
#include "../core/MyaiNode.h"
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
#include "../core/Edge.h"
>>>>>>> cf44269 (2025年2月23日 16:12:23)

#include <functional>
#include <map>


MYAI_BEGIN


<<<<<<< HEAD
<<<<<<< HEAD
=======
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


>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
/**
 * @brief 驱动
 */
class MyaiDriver {
<<<<<<< HEAD
<<<<<<< HEAD
public:
	using ptr					= std::shared_ptr<MyaiDriver>;
	using ControlConnectionFunc = std::function<void(weight_t)>;
	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;

	enum Type {
		DT_MEMORY,
=======
	friend class DriverManager;

=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
public:
	using ptr					= std::shared_ptr<MyaiDriver>;
	using ControlConnectionFunc = std::function<void(weight_t)>;
	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;

	enum Type {
<<<<<<< HEAD
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
		DT_MEMORY,
>>>>>>> cf44269 (2025年2月23日 16:12:23)
		DT_STATUS,
		DT_STRING,
		DT_AUDIO,
		DT_SCREEN_VIDEO,
		DT_CAMERA_VIDEO,
<<<<<<< HEAD
<<<<<<< HEAD

		__DT_END__
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======

		__DT_END__
>>>>>>> cf44269 (2025年2月23日 16:12:23)
	};


	MyaiDriver(Type type, nodeid_t begin, size_t id_size)
<<<<<<< HEAD
<<<<<<< HEAD
		: m_type(type), m_begin(begin), m_id_size(id_size), m_collects(std::make_shared<EdgeList>()) {
=======
		: m_type(type), m_begin(begin), m_id_size(id_size) {
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
		: m_type(type), m_begin(begin), m_id_size(id_size), m_collects(std::make_shared<EdgeList>()) {
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	}

	virtual ~MyaiDriver() = default;

	void init() {
		regeiste_controls();
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/// @brief 收集链接
	/// @return 链接列表
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
	EdgeList::ptr collect() {
		collect_data();
		return m_collects;
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	auto getCollects() { return m_collects; }

protected:
	using super						 = MyaiDriver;

<<<<<<< HEAD
=======

=======
>>>>>>> cf44269 (2025年2月23日 16:12:23)
protected:
<<<<<<< HEAD
	using super						 = MyaiDriver;
	using ControlConnectionFunc		 = std::function<void(weight_t)>;
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)

	virtual void collect_data()		 = 0;
	virtual void regeiste_controls() = 0;

<<<<<<< HEAD
=======
	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;

>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
	using super					= MyaiDriver;
	using ControlConnectionFunc = std::function<void(weight_t)>;
	static std::map<nodeid_t, ControlConnectionFunc> S_CONNECTIONS;
=======

>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
	virtual void collect_data()		 = 0;
	virtual void regeiste_controls() = 0;

>>>>>>> f902b12 (2025年2月23日 16:26:30)

protected:
	Type m_type;
	nodeid_t m_begin;
	size_t m_id_size;

	EdgeList::ptr m_collects;
};

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

<<<<<<< HEAD
=======
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
=======
=======

>>>>>>> f902b12 (2025年2月23日 16:26:30)
class MemoryDriver : public MyaiDriver {
	friend class MyaiDriverManager;

public:
	MemoryDriver(nodeid_t begin, size_t driver_size) : MyaiDriver(Type::DT_MEMORY, begin, 0) {}

private:
	virtual void collect_data() override {}
	virtual void regeiste_controls() override {}
};

>>>>>>> cf44269 (2025年2月23日 16:12:23)
=======
>>>>>>> ad9ab63 (2025年2月24日 19:48:47)
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

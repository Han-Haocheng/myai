#pragma once

#include "../driver/MyaiDriver.h"

MYAI_BEGIN

class MyaiDriverManager {
public:
	using ptr	 = std::shared_ptr<MyaiDriverManager>;
	// using Motive = void(const Link &);

	bool loadConfig();
	bool saveConfig();

	MyaiDriver::ptr addDriver(MyaiDriver::ptr driver);

	LinkList &&collect();
	void control(LinkList &output);

private:
	std::vector<MyaiDriver::ptr> m_drivers;
};

MYAI_END
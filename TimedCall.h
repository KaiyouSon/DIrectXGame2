#pragma once
#include <functional>
class TimedCall
{
private:
	std::function <void()>f;
	uint32_t time;
	bool isEnd = false;
public:
	TimedCall(std::function<void()> f, uint32_t time);
	void Update();
	bool GetisEnd();
};


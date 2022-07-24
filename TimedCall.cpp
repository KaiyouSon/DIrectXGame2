#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> f, uint32_t time) :
	f(f), time(time), isEnd(false)
{
}

void TimedCall::Update()
{
	if (isEnd == true) return;

	time--;
	if (time <= 0)
	{
		isEnd = true;
		f();
	}
}

bool TimedCall::GetisEnd()
{
	return isEnd;
}

#pragma once
#include <list>
#include <functional>


class Timer
{
private:
	struct QUEUE_ELEMENT
	{
		std::function<void()> on;
		float timeLeft;
	};

public:
	static void AddAram(const float _time, const std::function<void()>& _callback);

	static void Instance();

public:
	void Update();

private:
	Timer()
	{}
	~Timer()
	{}

private:
	std::list<QUEUE_ELEMENT> timerQueue_;
};

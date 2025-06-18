#pragma once
#include <list>
#include <functional>
#include <map>
#include <cstdint>


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
	static void AddInterval(const float _time, const std::function<void()>& _callback);

	static Timer& Instance();

public:
	void Update();

private:
	Timer()
	{}
	~Timer()
	{}

	void EnqueueTimer(QUEUE_ELEMENT* _pElement);

private:
	std::list<QUEUE_ELEMENT*> pTimerQueue_;  // タイマーキュー
	std::map<QUEUE_ELEMENT*, float> pReenqueueElements_;  // 使いまわしする要素のタイマー情報
};

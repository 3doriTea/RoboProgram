#include "Timer.h"

void Timer::AddAram(const float _time, const std::function<void()>& _callback)
{
	QUEUE_ELEMENT* pElement{ new QUEUE_ELEMENT{ _callback, _time } };

	float lefter{ _time };  // 減算用
	float righter{ 0 };                   // 加算用
	// 適切な挿入ポイントを見つける
	for (auto itr = Instance().timerQueue_.begin();
		itr != Instance().timerQueue_.end();
		itr++)
	{
		if (lefter <= righter + (*itr)->timeLeft)
		{
			pElement->timeLeft = lefter - righter;
			itr = Instance().timerQueue_.insert(itr, pElement);
			itr++;
			(*itr)->timeLeft -= pElement->timeLeft;
			return;
		}
		righter += (*itr)->timeLeft;
	}
	// 見つからなかったら末端に追加
	Instance().timerQueue_.push_back(pElement);
}

Timer& Timer::Instance()
{
	static Timer* pInstance{ new Timer{} };
	
	return *pInstance;
}

void Timer::Update()
{
}

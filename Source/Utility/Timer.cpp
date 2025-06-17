#include "Timer.h"

void Timer::AddAram(const float _time, const std::function<void()>& _callback)
{
	QUEUE_ELEMENT* pElement{ new QUEUE_ELEMENT{ _callback, _time } };

	float lefter{ _time };  // ���Z�p
	float righter{ 0 };                   // ���Z�p
	// �K�؂ȑ}���|�C���g��������
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
	// ������Ȃ������疖�[�ɒǉ�
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

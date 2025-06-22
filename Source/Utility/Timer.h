#pragma once
#include <list>
#include <functional>
#include <map>
#include <cstdint>


using TimerHandle = void*;

class Timer
{
private:
	struct QUEUE_ELEMENT
	{
		std::function<void()> on;
		float timeLeft;
	};

public:
	static TimerHandle AddAram(const float _time, const std::function<void()>& _callback);
	static TimerHandle AddInterval(const float _time, const std::function<void()>& _callback, const bool _firstCall = false);
	static void Remove(TimerHandle _hTimer);

	/// <summary>
	/// �^�C�}�[�L���[�����ׂăN���A����
	/// </summary>
	static void Clear();

	static Timer& Instance();

	static void Release();
public:
	void Update();

private:
	Timer()
	{}
	~Timer();

	void EnqueueTimer(QUEUE_ELEMENT* _pElement);

private:
	static Timer* pInstance_;
	std::list<QUEUE_ELEMENT*> pTimerQueue_;  // �^�C�}�[�L���[
	std::map<QUEUE_ELEMENT*, float> pReenqueueElements_;  // �g���܂킵����v�f�̃^�C�}�[���
};

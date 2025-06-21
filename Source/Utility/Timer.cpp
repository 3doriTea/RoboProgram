#include "Timer.h"
#include "../../Library/Time.h"
#include <DxLib.h>


void Timer::AddAram(const float _time, const std::function<void()>& _callback)
{
	Instance().EnqueueTimer(new QUEUE_ELEMENT{ _callback, _time });
}

void Timer::AddInterval(const float _time, const std::function<void()>& _callback)
{
	QUEUE_ELEMENT* pElement{ new QUEUE_ELEMENT{ _callback, _time } };
	Instance().pReenqueueElements_.insert({ pElement, _time });
	Instance().EnqueueTimer(pElement);
}

void Timer::Clear()
{
	for (auto& pTimer : Instance().pTimerQueue_)
	{
		delete pTimer;
	}

	Instance().pTimerQueue_.clear();
}

Timer& Timer::Instance()
{
	if (pInstance_ == nullptr)
	{
		pInstance_ = new Timer{};
	}
	
	return *pInstance_;
}

void Timer::Release()
{
	Instance().Clear();
	if (pInstance_ != nullptr)
	{
		delete pInstance_;
		pInstance_ = nullptr;
	}
}

void Timer::Update()
{
	if (pTimerQueue_.size() <= 0)
	{
		return;  // �^�C�}�[�L���[����Ȃ��A
	}

	auto itr = pTimerQueue_.begin();
	while (true)
	{
		(*itr)->timeLeft -= Time::DeltaTime();

		float diff{ (*itr)->timeLeft };  // ����
		if (diff > 0)  // ������0���傫�� = �ҋ@��
		{
			break;  // �^�C�}�[�I�����Ă��Ȃ����ߗ��E
		}
		else  // ������0�ȉ� = �^�C�}�[�I��
		{
			(*itr)->on();  // ���s

			// �����g���܂킵����v�f�Ȃ�
			if (pReenqueueElements_.count(*itr) > 0)
			{
				QUEUE_ELEMENT* pElement = *itr;
				(*itr)->timeLeft = pReenqueueElements_[*itr];
				itr = pTimerQueue_.erase(pTimerQueue_.begin());  // ����
				EnqueueTimer(pElement);  // ���ԃZ�b�g���ăG���L���[
				break;
			}
			// �g���܂킳�Ȃ��Ȃ�
			{
				delete (*itr);  // �������
				itr = pTimerQueue_.erase(pTimerQueue_.begin());  // ����
			}
			if (itr == pTimerQueue_.end())
			{
				break;  // �㑱���Ȃ� = �L���[����Ȃ痣�E
			}
			(*itr)->timeLeft += diff;  // �������㑱�ɔ��f
			continue;  // �㑱���I�����Ă���\�������邽�ߌp��
		}
	}
}

Timer::~Timer()
{
	for (auto& pTimer : Instance().pTimerQueue_)
	{
		delete pTimer;
	}
}

void Timer::EnqueueTimer(QUEUE_ELEMENT* _pElement)
{
	float lefter{ _pElement->timeLeft };  // ���Z�p
	float righter{ 0 };     // ���Z�p
	// �K�؂ȑ}���|�C���g��������
	for (auto itr = Instance().pTimerQueue_.begin();
		itr != Instance().pTimerQueue_.end();
		itr++)
	{
		if (lefter <= righter + (*itr)->timeLeft)
		{
			_pElement->timeLeft = lefter - righter;
			itr = Instance().pTimerQueue_.insert(itr, _pElement);
			itr++;
			(*itr)->timeLeft -= _pElement->timeLeft;
			return;
		}
		righter += (*itr)->timeLeft;
	}
	// ������Ȃ������疖�[�ɒǉ�
	Instance().pTimerQueue_.push_back(_pElement);
}

Timer* Timer::pInstance_{ nullptr };

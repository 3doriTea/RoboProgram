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
		return;  // タイマーキューが空なら回帰
	}

	auto itr = pTimerQueue_.begin();
	while (true)
	{
		(*itr)->timeLeft -= Time::DeltaTime();

		float diff{ (*itr)->timeLeft };  // 差分
		if (diff > 0)  // 差分が0より大きい = 待機中
		{
			break;  // タイマー終了していないため離脱
		}
		else  // 差分が0以下 = タイマー終了
		{
			(*itr)->on();  // 実行

			// もし使いまわしする要素なら
			if (pReenqueueElements_.count(*itr) > 0)
			{
				QUEUE_ELEMENT* pElement = *itr;
				(*itr)->timeLeft = pReenqueueElements_[*itr];
				itr = pTimerQueue_.erase(pTimerQueue_.begin());  // 消す
				EnqueueTimer(pElement);  // 時間セットしてエンキュー
				break;
			}
			// 使いまわさないなら
			{
				delete (*itr);  // 解放する
				itr = pTimerQueue_.erase(pTimerQueue_.begin());  // 消す
			}
			if (itr == pTimerQueue_.end())
			{
				break;  // 後続がない = キューが空なら離脱
			}
			(*itr)->timeLeft += diff;  // 差分を後続に反映
			continue;  // 後続も終了している可能性があるため継続
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
	float lefter{ _pElement->timeLeft };  // 減算用
	float righter{ 0 };     // 加算用
	// 適切な挿入ポイントを見つける
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
	// 見つからなかったら末端に追加
	Instance().pTimerQueue_.push_back(_pElement);
}

Timer* Timer::pInstance_{ nullptr };

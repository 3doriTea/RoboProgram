#include "Fader.h"
#include <cassert>
#include "../Library/Time.h"


Fader::Fader(const char* _imageFileName, const float _fadeTime, const bool _showToHide) :
	FADE_TIME{ _fadeTime },
	timeLeft_{ _fadeTime },
	showToHide_{ _showToHide },
	fadeType_{ FadeType::Ghost }
{
	hImage_ = LoadGraph(_imageFileName);
	assert(hImage_ != -1 && "フェード画像読み込みに失敗");

	SetDrawOrder(-1000);
}

Fader::~Fader()
{
}

void Fader::Update()
{
	timeLeft_ -= Time::DeltaTime();
	if (timeLeft_ <= 0)
	{
		timeLeft_ = 0.0f;
		onFinish_();
		DestroyMe();
	}
}

void Fader::Draw()
{
	float larp{ timeLeft_ / FADE_TIME };
	if (showToHide_ == false)
	{
		larp = 1.0f - larp;
	}

	switch (fadeType_)
	{
	case Fader::FadeType::Ghost:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, larp * UINT8_MAX);
		DrawGraph(0, 0, hImage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}
}

#include "StartButton.h"
#include <cassert>
#include "IO/Input.h"
#include "Utility/RectanUtility.h"


namespace
{
	static const char FADE_IMAGE_FILE[]{ "Data/Image/background.png" };
	static const int DRAW_ORDER{ -10 };
	static const int ON_HIT_COLOR_ALPHA{ 200 };
}

StartButton::StartButton(const char* _imageFile) : 
	alpha_{ INT8_MAX },
	pushed_{ false }
{
	hImage_ = LoadGraph(_imageFile);
	assert(hImage_ != -1 && "É{É^ÉìâÊëúì«Ç›çûÇ›Ç…é∏îs");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	SetDrawOrder(DRAW_ORDER);
}

StartButton::~StartButton()
{
}

void StartButton::Update()
{
	Input::Instance().Update();
	if (pushed_)
	{
		return;
	}

	Vector2Int mousePos{};
	GetMousePoint(&mousePos.x, &mousePos.y);

	if (RectanUtility::IsHit(rect_, Vector2{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) }))
	{
		alpha_ = ON_HIT_COLOR_ALPHA;
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			pushed_ = true;
			(new Fader{ FADE_IMAGE_FILE, 1.0f, false })->OnFinish([&, this]()
				{
					SceneManager::ChangeScene("PLAY");
				});
		}
	}
	else
	{
		alpha_ = 0;
	}
}

void StartButton::Draw()
{
	DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha_);
	DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

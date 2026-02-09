#include "WriteButton.h"
#include "PlayScene.h"
#include <cassert>
#include "Screen.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/WriteButton.png" };
	static const int ALPHA_BLEND_VALUE{ 100 };
	static const int NO_BLEND_VALUE{ 0 };
}

WriteButton::WriteButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "書き込みボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = static_cast<float>(Screen::WIDTH - ((rect_.width + BUTTON_PADDING) * 2));
	rect_.y = static_cast<float>(BUTTON_PADDING);

	SetHintText("コードを編集する");
}

WriteButton::~WriteButton()
{
}

void WriteButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NO_BLEND_VALUE);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_BLEND_VALUE);
	}
	DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, ALPHA_BLEND_VALUE);
		DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NO_BLEND_VALUE);
}

void WriteButton::OnPush()
{
	GetScene<PlayScene>()->OpenSrcFile();
}

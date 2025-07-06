#include "WriteButton.h"
#include "PlayScene.h"
#include <cassert>
#include "Screen.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/WriteButton.png" };
}

WriteButton::WriteButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "書き込みボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = Screen::WIDTH - ((rect_.width + BUTTON_PADDING) * 2);
	rect_.y = BUTTON_PADDING;

	SetHintText("コードを編集する");
}

WriteButton::~WriteButton()
{
}

void WriteButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	DrawGraph(rect_.x, rect_.y, hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void WriteButton::OnPush()
{
	GetScene<PlayScene>()->OpenSrcFile();
}

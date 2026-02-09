#include "ReadCodeButton.h"
#include "ClearScene.h"
#include <cassert>
#include "Screen.h"
#include "IO/FileSaver.h"
#include "PlayScene.h"


namespace
{
	static const char BUTTON_IMAGE[]{ "Data/Image/ReadCodeButton.png" };
	static const char CODE_TEXT_NAME[]{ "ReadOnlySourceCode.txt" };
	static const int BUTTON_POS_X{ 740 }; // コード読み取りボタンの座標x
	static const int BUTTON_POS_Y{ 570 }; // コード読み取りボタンの座標y

	static const int ADD_BLEND_VALUE{ 100 };
	static const int NO_BLEND_VALUE{ 0 };
}

ReadCodeButton::ReadCodeButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "ボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = BUTTON_POS_X;
	rect_.y = BUTTON_POS_Y;

	SetHintText("ゴールしたソースコードを表示します。");
}

ReadCodeButton::~ReadCodeButton()
{
}

void ReadCodeButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ADD, ADD_BLEND_VALUE);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NO_BLEND_VALUE);
	}
	DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, ADD_BLEND_VALUE);
		DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NO_BLEND_VALUE);
}

void ReadCodeButton::OnPush()
{
	FileSaver::QuickWriteText(CODE_TEXT_NAME, PlayScene::GetSrcCodeText());
	GetScene<ClearScene>()->OpenFile(CODE_TEXT_NAME);
}

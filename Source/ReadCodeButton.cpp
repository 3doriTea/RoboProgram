#include "ReadCodeButton.h"
#include "ClearScene.h"
#include <cassert>
#include "Screen.h"
#include "IO/FileSaver.h"
#include "PlayScene.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/ReadCodeButton.png" };
	static char CODE_TEXT_NAME[]{ "ReadOnlySourceCode.txt" };
}

ReadCodeButton::ReadCodeButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "ボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = 740;//Screen::WIDTH / 2 - rect_.width / 2;
	rect_.y = 570;//Screen::HEIGHT - ;

	SetHintText("ゴールしたソースコードを表示します。");
}

ReadCodeButton::~ReadCodeButton()
{
}

void ReadCodeButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	DrawGraph(rect_.x, rect_.y, hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ReadCodeButton::OnPush()
{
	FileSaver::QuickWriteText(CODE_TEXT_NAME, PlayScene::GetSrcCodeText());
	GetScene<ClearScene>()->OpenFile(CODE_TEXT_NAME);
}

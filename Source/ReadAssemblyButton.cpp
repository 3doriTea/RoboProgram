#include "ReadAssemblyButton.h"
#include "ClearScene.h"
#include <cassert>
#include "Screen.h"
#include "IO/FileSaver.h"
#include "PlayScene.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/ReadAssemblyButton.png" };
	static char ASSEMBLE_TEXT_NAME[]{ "ByteCodeAndAssembler.txt" };
}

ReadAssemblyButton::ReadAssemblyButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "ボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = 130;//Screen::WIDTH / 2 - rect_.width / 2;
	rect_.y = 570;//Screen::HEIGHT - ;

	SetHintText("コンピュータが理解できる形式で表示します。");
}

ReadAssemblyButton::~ReadAssemblyButton()
{
}

void ReadAssemblyButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
		DrawGraph(static_cast<int>(rect_.x), static_cast<int>(rect_.y), hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ReadAssemblyButton::OnPush()
{
	FileSaver::QuickWriteText(ASSEMBLE_TEXT_NAME, PlayScene::GetAssembleText());
	GetScene<ClearScene>()->OpenFile(ASSEMBLE_TEXT_NAME);
}

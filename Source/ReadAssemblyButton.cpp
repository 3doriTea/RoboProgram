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
	assert(hImage_ != 0 && "�{�^���摜�ǂݍ��݂Ɏ��s");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = Screen::WIDTH - ((rect_.width + BUTTON_PADDING) * 2);
	rect_.y = BUTTON_PADDING;

	SetHintText("�R���s���[�^�������ł���`���ŕ\�����܂��B");
}

ReadAssemblyButton::~ReadAssemblyButton()
{
}

void ReadAssemblyButton::OnDraw(const bool _onTouching, const bool _isPushing)
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

void ReadAssemblyButton::OnPush()
{
	FileSaver::QuickWriteText(ASSEMBLE_TEXT_NAME, PlayScene::GetAssembleText());
	GetScene<ClearScene>()->OpenFile(ASSEMBLE_TEXT_NAME);
}

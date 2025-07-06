#include "DocButton.h"
#include <cassert>
#include "Stage.h"
#include "Screen.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/DocButton.png" };
	static char BUTTON_IMAGE_NEW[]{ "Data/Image/DocButton-new.png" };
}

DocButton::DocButton()
{
	hImageNormal_ = LoadGraph(BUTTON_IMAGE);
	assert(hImageNormal_ != 0 && "�h�L�������g�{�^�� �m�[�}�� �摜�ǂݍ��݂Ɏ��s");
	hImageNew_ = LoadGraph(BUTTON_IMAGE_NEW);
	assert(hImageNew_ != 0 && "�h�L�������g�{�^�� �V�K �摜�ǂݍ��݂Ɏ��s");

	hImage_ = hImageNormal_;

	GetGraphSizeF(hImageNormal_, &rect_.width, &rect_.height);

	rect_.x = Screen::WIDTH - (rect_.width * 1);
	rect_.y = 0;

	SetDrawOrder(-110);

	SetHintText("�h�L�������g������");
}

DocButton::~DocButton()
{
}

void DocButton::OnDraw(const bool _onTouching, const bool _isPushing)
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

void DocButton::OnPush()
{
	hImage_ = hImageNormal_;
	pStage_->OpenDocument();
}

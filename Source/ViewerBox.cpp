#include "ViewerBox.h"
#include <cassert>


static const ColorCode COLOR_BLACK{ 0x000000 };  // �f�t�H���g�̍�

ViewerBox::ViewerBox() :
	readingLine_{ -1 },
	frameWidth_{ 0 },
	frameColor_{ COLOR_BLACK },
	defaultBackgroundColor_{ COLOR_BLACK },
	defaultTextColor_{ COLOR_BLACK },
	textLines_{},
	lineMarks_{}
{
}

ViewerBox::~ViewerBox()
{
}

void ViewerBox::Update()
{
}

void ViewerBox::Draw()
{
}

void ViewerBox::ReadLine(const int _line)
{
	assert(-1 <= _line && _line <= textLines_.size()
		&& "�ǂݎ��ꏊ���͈͊O�ł� @ViewerBox::ReadLine");
	readingLine_ = _line;
}

ViewerBox& ViewerBox::ClearMarks()
{
	lineMarks_.clear();

	return *this;
}

ViewerBox& ViewerBox::AddMarkLine(const int _line, const BackgroundColor _backgroundColor, const TextColor _textColor)
{
	assert(lineMarks_.count(_line) <= 0
		&& "�}�[�N���C�����d�����Ă��܂� @ViewerBox::AddMarkLine");

	lineMarks_.insert({ _line, { _backgroundColor, _textColor }});

	return *this;
}

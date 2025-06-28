#include "ViewerBox.h"
#include <cassert>


ViewerBox::ViewerBox()
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
	assert(-1 <= _line && _line <= lineTexts_.size()
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
}

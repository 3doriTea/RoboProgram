#include "ViewerBox.h"
#include <cassert>


static const ColorCode COLOR_BLACK{ 0xffffff };  // デフォルトの黒
static const int TEXT_BOX_MARGIN{ 4 };
static const int TEXT_LINE_MARGIN{ 4 };
static const ColorCode START_FRAME_COLOR{ 0x000000 };
static const ColorCode START_DEFAULT_BACKGROUND_COLOR{ 0xffffff };
static const ColorCode START_DEFAULT_BACKGROUND_COLOR{ 0xffffff };

ViewerBox::ViewerBox() :
	pivotType_{ Pivot::TopLeft },
	readingLine_{ -1 },
	frameWidth_{ 0 },
	frameColor_{ COLOR_BLACK },
	defaultBackgroundColor_{ COLOR_BLACK },
	defaultTextColor_{ COLOR_BLACK },
	maxShowLine_{ 0 },
	lineMarginSize_{ TEXT_LINE_MARGIN },
	textBoxMargin_{ TEXT_BOX_MARGIN },
	textLines_{},
	lineMarks_{},
	lineSize_{},
	lineCount_{}
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
	//printfDx("%d\n", defaultBackgroundColor_);
	DrawBox(
		rect_.x,
		rect_.y,
		rect_.x + rect_.width,
		rect_.y + rect_.height,
		defaultBackgroundColor_,
		TRUE);

	int beginLine = 0;
	int endLine = textLines_.size();

	/*if (maxShowLine_ > 0)
	{
		
	}*/

	for (int l = beginLine; l < endLine; l++)
	{
		if (l < 0 && endLine <= l)
		{
			continue;  // 範囲外なら
		}
		
		BackgroundColor bgColor{ defaultBackgroundColor_ };
		TextColor textColor{ defaultTextColor_ };
		
		if (lineMarks_.count(l) > 0)
		{
			bgColor = lineMarks_[l].first;
			textColor = lineMarks_[l].second;
		}

		/*DrawBox(
			rect_.x, rect_.y + (lineSize_ + lineMarginSize_) * (l - beginLine),
			rect_.x + rect_.width, rect_.y + (lineSize_ + lineMarginSize_) * (l - beginLine + 1),
			bgColor, TRUE);*/
		/*DrawFormatString(
			rect_.x,
			rect_.y + (lineSize_ + lineMarginSize_) * (l - beginLine),
			textColor,
			"%s",
			textLines_[l].c_str());*/
	}
}

ViewerBox& ViewerBox::SetTextLines(const std::vector<std::string>& _textLines)
{
	textLines_ = _textLines;
	std::string str{};

	for (int l = 0; l < textLines_.size(); l++)
	{
		str += textLines_[l];
		if (l != textLines_.size() - 1)
		{
			str += "\n";
		}
	}

	GetDrawFormatStringSize(&textBoxSize_.x, &textBoxSize_.y, &lineCount_, "%s", str.c_str());
	lineSize_ = GetFontSize();

	rect_.width = textBoxSize_.x;
	rect_.height = textLines_.size() * (lineSize_ + lineMarginSize_);

	return *this;
}

void ViewerBox::ReadLine(const int _line)
{
	assert(-1 <= _line && _line <= textLines_.size()
		&& "読み取り場所が範囲外です @ViewerBox::ReadLine");
	readingLine_ = _line;
}

void ViewerBox::Focus(const int _line)
{
	readingLine_ = _line;
}

void ViewerBox::SetPosition(const Vector2Int _position, const Pivot _pivotType)
{
	rect_.pivot = _position;
	pivotType_ = _pivotType;
}

ViewerBox& ViewerBox::ClearMarks()
{
	lineMarks_.clear();

	return *this;
}

ViewerBox& ViewerBox::AddMarkLine(const int _line, const BackgroundColor _backgroundColor, const TextColor _textColor)
{
	assert(lineMarks_.count(_line) <= 0
		&& "マークラインが重複しています @ViewerBox::AddMarkLine");

	lineMarks_.insert({ _line, { _backgroundColor, _textColor }});

	return *this;
}

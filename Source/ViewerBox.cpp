#include "ViewerBox.h"
#include <cassert>
#include "Utility/RectanUtility.h"
#include "Screen.h"
#include "IO/Input.h"


namespace
{
	static const ColorCode COLOR_BLACK{ 0x000000 };  // デフォルトの黒
	static const ColorCode COLOR_WHITE{ 0xffffff };  // デフォルトの白
	
	static const int TEXT_BOX_MARGIN{ 4 };
	static const int TEXT_LINE_MARGIN{ 4 };
	static const int GHOST_ALPHA{ 80 };
	static const ColorCode START_FRAME_COLOR{ COLOR_BLACK };
	static const ColorCode START_DEFAULT_BACKGROUND_COLOR{ COLOR_WHITE };
	static const ColorCode START_DEFAULT_TEXT_COLOR{ COLOR_BLACK };
}

ViewerBox::ViewerBox() :
	pivotType_{ Pivot::TopLeft },
	readingLine_{ -1 },
	frameWidth_{ 0 },
	frameColor_{ START_FRAME_COLOR },
	defaultBackgroundColor_{ START_DEFAULT_BACKGROUND_COLOR },
	defaultTextColor_{ START_DEFAULT_TEXT_COLOR },
	showLineCount_{ 0 },
	lineMarginSize_{ TEXT_LINE_MARGIN },
	textBoxMargin_{ TEXT_BOX_MARGIN },
	textLines_{},
	lineMarks_{},
	lineSize_{},
	lineCount_{},
	drawAlpha_{ UINT8_MAX },
	isScrollable_{ false },
	isShowLineCountBar_{ false },
	isShow_{ true },
	useGhost_{ false },
	onClick_{[]{}}
{
}

ViewerBox::~ViewerBox()
{
}

void ViewerBox::Update()
{
	if (isShow_ == false && isScrollable_ == false)
	{
		return;
	}

	Vector2Int mousePos{};
	GetMousePoint(&mousePos.x, &mousePos.y);
	if (RectanUtility::IsHit(rect_, Vector2{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) }))
	{
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			onClick_();
		}

		int volume{ GetMouseWheelRotVol() };
		readingLine_ += -volume;
		if (readingLine_ < 0)
		{
			readingLine_ = 0;
		}
		else if (readingLine_ >= textLines_.size())
		{
			readingLine_ = static_cast<int>(textLines_.size() - 1);
		}
		drawAlpha_ = UINT8_MAX;
	}
	else
	{
		drawAlpha_ = GHOST_ALPHA;
	}
}

void ViewerBox::Draw()
{
	if (isShow_ == false)
	{
		return;
	}

	if (useGhost_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, drawAlpha_);
	}
	DrawBox(  // 枠表示
		static_cast<int>(rect_.x) - frameWidth_,
		static_cast<int>(rect_.y) - frameWidth_,
		static_cast<int>(rect_.x + rect_.width) + frameWidth_,
		static_cast<int>(rect_.y + rect_.height) + frameWidth_,
		frameColor_,
		TRUE);

	DrawBox(  // 背景表示
		static_cast<int>(rect_.x),
		static_cast<int>(rect_.y),
		static_cast<int>(rect_.x + rect_.width),
		static_cast<int>(rect_.y + rect_.height),
		defaultBackgroundColor_,
		TRUE);

	int beginLine = 0;
	int endLine = static_cast<int>(textLines_.size());

	if (showLineCount_ > 0)
	{
		int topLineCount{ showLineCount_ / 2 };
		beginLine = readingLine_ - topLineCount;
		if (beginLine < 0)
		{
			beginLine = 0;
		}
		endLine = beginLine + showLineCount_;
	}

	for (int l = beginLine; l < endLine; l++)
	{
		if (l < 0 || textLines_.size() <= l)
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

		DrawBox(
			static_cast<int>(rect_.x) + textBoxMargin_, static_cast<int>(rect_.y) + (lineSize_ + lineMarginSize_) * (l - beginLine),
			static_cast<int>(rect_.x) + static_cast<int>(rect_.width) - textBoxMargin_, static_cast<int>(rect_.y) + (lineSize_ + lineMarginSize_) * (l - beginLine + 1),
			bgColor, TRUE);
		if (isShowLineCountBar_)
		{
			DrawFormatString(
				static_cast<int>(rect_.x) + textBoxMargin_,
				static_cast<int>(rect_.y) + textBoxMargin_ + (lineSize_ + lineMarginSize_) * (l - beginLine),
				textColor,
				"%2d:%s",
				l,
				textLines_[l].c_str());
		}
		else
		{
			DrawFormatString(
				static_cast<int>(rect_.x) + textBoxMargin_,
				static_cast<int>(rect_.y) + textBoxMargin_ + (lineSize_ + lineMarginSize_) * (l - beginLine),
				textColor,
				"%s",
				textLines_[l].c_str());
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

ViewerBox& ViewerBox::SetTextLines(const std::vector<std::string>& _textLines)
{
	textLines_ = _textLines;
	Recalculate();

	return *this;
}

ViewerBox& ViewerBox::ReadLine(const int _line)
{
	if (_line >= textLines_.size())
	{
		readingLine_ = -1;
	}
	else
	{
		readingLine_ = _line;
	}

	return Recalculate();
}

ViewerBox& ViewerBox::SetPosition(const Vector2Int _position, const Pivot _pivotType)
{
	// NOTE: 位置合わせは計算関数がしてくれるため、このままでOK
	position_ = _position;
	pivotType_ = _pivotType;

	return Recalculate();
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

	return Recalculate();
}

ViewerBox& ViewerBox::Recalculate()
{
	if (textLines_.size() <= 0)  // もしテキストが 0 表示しない！
	{
		rect_.size = { 0, 0 };
		isShow_ = false;
		return *this;
	}
	
	isShow_ = true;

	std::string str{};
	int maxCount{};  // 最大文字数
	int maxCountLine{};  // 最大文字数のライン

	for (int l = 0; l < textLines_.size(); l++)
	{
		str += textLines_[l];
		
		if (maxCount <= textLines_[l].size())
		{
			maxCount = static_cast<int>(textLines_[l].size());
			maxCountLine = l;
		}

		if (l != textLines_.size() - 1)
		{
			str += "\n";
		}
	}

	GetDrawFormatStringSize(&textBoxSize_.x, &textBoxSize_.y, &lineCount_, "%s", str.c_str());
	lineSize_ = GetFontSize();

	if (isShowLineCountBar_)
	{
		rect_.width = static_cast<float>(GetDrawFormatStringWidth(
			"%s:%s",
			std::to_string(textLines_.size()).c_str(),
			textLines_[maxCountLine].c_str()) + 30);
	}
	else
	{
		rect_.width = static_cast<float>(textBoxSize_.x);
	}
	rect_.width += textBoxMargin_ * 2;

	rect_.height = static_cast<float>(lineSize_ + lineMarginSize_);
	rect_.height *= (showLineCount_ == 0) ? textLines_.size() : showLineCount_;
	rect_.height += textBoxMargin_ * 2;

	switch (pivotType_)
	{
	case Pivot::TopLeft:
		rect_.pivot = position_;
		break;
	case Pivot::BottomRight:
		rect_.pivot = position_;
		rect_.pivot -= rect_.size;
		break;
	case Pivot::BottomLeft:
		rect_.pivot = position_;
		rect_.y -= rect_.height;
		break;
	default:
		assert(false);
		break;
	}

	return *this;
}

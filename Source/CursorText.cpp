#include "CursorText.h"
#include <cassert>
#include "Screen.h"


namespace
{
	static const int TEXT_BOX_PADDING{ 3 };
}

CursorText::CursorText() :
	text_{},
	mousePos_{},
	textBox_{}
{
	SetDrawOrder(-200);
}

CursorText::~CursorText()
{
}

void CursorText::Update()
{
	Vector2Int mousePos;
	GetMousePoint(&mousePos.x, &mousePos.y);
	mousePos_.x = static_cast<float>(mousePos.x);
	mousePos_.y = static_cast<float>(mousePos.y);
}

void CursorText::Draw()
{
	if (text_.size() <= 0)
	{
		return;
	}

	textBox_.pivot = mousePos_.ToInt();
	int lineCount{};
	GetDrawFormatStringSize(&textBox_.width, &textBox_.height, &lineCount, "%s", text_.c_str());
	assert(lineCount == 1 && "ƒqƒ“ƒg‚ª•¡”s‚¾‚Á‚½");

	textBox_.width += TEXT_BOX_PADDING * 2;
	textBox_.height += TEXT_BOX_PADDING * 2;

	if (textBox_.x + textBox_.width > Screen::WIDTH)
	{
		textBox_.x -= textBox_.width;
	}
	if (textBox_.y + textBox_.height > Screen::HEIGHT)
	{
		textBox_.y -= textBox_.height;
	}

	DrawBox(
		textBox_.x, textBox_.y,
		textBox_.x + textBox_.width, textBox_.y + textBox_.height,
		0xffffff, TRUE);
	DrawFormatString(
		textBox_.x + TEXT_BOX_PADDING,
		textBox_.y + TEXT_BOX_PADDING,
		0x000000,
		"%s", text_.c_str());

	text_ = "";
}

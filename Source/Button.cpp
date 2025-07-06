#include "Button.h"
#include "Utility/RectanUtility.h"
#include "IO/Input.h"
#include <cassert>
#include "Screen.h"


namespace
{
	static const int TEXT_BOX_PADDING{ 3 };
}


Button::Button() :
	Object2D{},
	isOnMouse_{ false },
	isPushing_{ false },
	hintText_{}
{
}

Button::~Button()
{
}

void Button::Update()
{
	Vector2Int mousePos;
	GetMousePoint(&mousePos.x, &mousePos.y);
	mousePos_.x = static_cast<float>(mousePos.x);
	mousePos_.y = static_cast<float>(mousePos.y);

	isOnMouse_ = RectanUtility::IsHit(rect_, mousePos_);

	if (isOnMouse_ && Input::IsMouseDown(MOUSE_INPUT_LEFT))
	{
		OnPush();
		isPushing_ = true;
	}

	if (isPushing_)
	{
		isPushing_ = Input::IsMouse(MOUSE_INPUT_LEFT);
	}
}

void Button::Draw()
{
	OnDraw(isOnMouse_, isPushing_);

	if (isOnMouse_ && hintText_.size() > 0)
	{
		RectInt textBoxRect{};
		textBoxRect.pivot = mousePos_.ToInt();
		int lineCount{};
		GetDrawFormatStringSize(&textBoxRect.width, &textBoxRect.height, &lineCount, "%s", hintText_.c_str());
		assert(lineCount == 1 && "ƒqƒ“ƒg‚ª•¡”s‚¾‚Á‚½");

		textBoxRect.width += TEXT_BOX_PADDING * 2;
		textBoxRect.height += TEXT_BOX_PADDING * 2;

		if (textBoxRect.x + textBoxRect.width > Screen::WIDTH)
		{
			textBoxRect.x -= textBoxRect.width;
		}
		if (textBoxRect.y + textBoxRect.height > Screen::HEIGHT)
		{
			textBoxRect.y -= textBoxRect.height;
		}

		DrawBox(
			textBoxRect.x, textBoxRect.y,
			textBoxRect.x + textBoxRect.width, textBoxRect.y + textBoxRect.height,
			0xffffff, TRUE);
		DrawFormatString(
			textBoxRect.x + TEXT_BOX_PADDING,
			textBoxRect.y + TEXT_BOX_PADDING,
			0x000000,
			"%s", hintText_.c_str());

	}
}

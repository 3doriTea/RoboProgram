#include "Button.h"
#include "Utility/RectanUtility.h"
#include "IO/Input.h"
#include <cassert>
#include "CursorText.h"


const int Button::BUTTON_PADDING{ 10 };

Button::Button() :
	Object2D{},
	isOnMouse_{ false },
	isPushing_{ false },
	hintText_{},
	pCursorText_{ FindGameObject<CursorText>() }
{
	SetDrawOrder(-110);
}

Button::~Button()
{
}

void Button::Update()
{
	Vector2Int mousePos;
	GetMousePoint(&mousePos.x, &mousePos.y);
	isOnMouse_ = RectanUtility::IsHit(rect_, { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });

	if (isOnMouse_ && Input::IsMouseDown(MOUSE_INPUT_LEFT))
	{
		//SetDrawOrder(-1);
		OnPush();
		isPushing_ = true;
	}
	//SetDrawOrder(-110);

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
		if (pCursorText_ != nullptr)
		{
			pCursorText_->SetText(hintText_);
		}
	}
}

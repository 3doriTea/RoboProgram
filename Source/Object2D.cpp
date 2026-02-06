#include "Object2D.h"
#include "Stage.h"


Object2D::Object2D() :
	hImage_{ -1 },
	rect_{},
	pStage_{ FindGameObject<Stage>() }
{
}

Object2D::~Object2D()
{
}

void Object2D::Update()
{
}

void Object2D::Draw()
{
	if (hImage_ > 0)
	{
		int width{};
		GetGraphSize(hImage_, &width, nullptr);
		int x = static_cast<int>(rect_.x);
		int y = static_cast<int>(rect_.y);

		Vector2 scroll{};
		if (pStage_ != nullptr)
		{
			scroll = pStage_->GetScroll();
		}
		DrawGraph(static_cast<int>(x - scroll.x - (width - rect_.width) / 2.0f), static_cast<int>(y - scroll.y), hImage_, TRUE);
	}
}

Rectan Object2D::GetRect() const
{
	Vector2 scroll{};
	if (pStage_ != nullptr)
	{
		scroll = pStage_->GetScroll();
	}
	Rectan drawRect{};
	drawRect.x = rect_.x - scroll.x;
	drawRect.y = rect_.y - scroll.y,
	drawRect.size = rect_.size;
	return drawRect;
}

Vector2 Object2D::ToWorld(const Vector2& _localPosition) const
{
	Vector2 scroll{};
	if (pStage_ != nullptr)
	{
		scroll = pStage_->GetScroll();
	}
	
	return rect_.GetCenter() + _localPosition - scroll;
}

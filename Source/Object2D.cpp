#include "Object2D.h"
#include "Stage.h"


namespace
{
}

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
		int x = rect_.x;
		int y = rect_.y;

		Vector2 scroll{};
		if (pStage_ != nullptr)
		{
			scroll = pStage_->GetScroll();
		}
		//DrawRectGraph(x - scroll.x, y - scroll.y, anim * imageSize.x, animY * imageSize.y, imageSize.x, imageSize.y, hImage, TRUE);
		//DrawRectGraph(x - scroll.x, y - scroll.y, 0, 0, IMAGE_SIZE, IMAGE_SIZE, hImage_, TRUE);
		DrawGraph(x - scroll.x - (width - rect_.width) / 2, y - scroll.y, hImage_, TRUE);

		#if false
		DrawBox(
			x - scroll.x, y - scroll.y,
			x - scroll.x + rect_.width, y - scroll.y + rect_.height,
			0xff00ff, FALSE);
		#endif
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

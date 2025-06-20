#include "Object2D.h"
#include "Stage.h"


namespace
{
	static const int IMAGE_SIZE{ 80 };
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
		int x = rect_.GetCenter().x - IMAGE_SIZE / 2;
		int y = rect_.GetCenter().y - IMAGE_SIZE / 2;

		Vector2 scroll{};
		if (pStage_ != nullptr)
		{
			scroll = pStage_->GetScroll();
		}
		//DrawRectGraph(x - scroll.x, y - scroll.y, anim * imageSize.x, animY * imageSize.y, imageSize.x, imageSize.y, hImage, TRUE);
		DrawRectGraph(x - scroll.x, y - scroll.y, 0, 0, IMAGE_SIZE, IMAGE_SIZE, hImage_, TRUE);
	}
}

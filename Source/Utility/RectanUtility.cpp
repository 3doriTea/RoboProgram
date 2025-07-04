#include "RectanUtility.h"
#include <cmath>

bool RectanUtility::IsHit(const Rectan& _rect, const Vector2& _p)
{
	Vector2 max{ _rect.GetMax() };
	Vector2 min{ _rect.GetMin() };

	return
	{
		min.x <= _p.x && _p.x <= max.x
		&&
		min.y <= _p.y && _p.y <= max.y
	};
}

bool RectanUtility::IsHit(const RectInt& _rect1, const RectInt& _rect2)
{
	int rect1CenterX{ _rect1.x + _rect1.width / 2 };
	int rect1CenterY{ _rect1.y + _rect1.height / 2 };

	int rect2CenterX{ _rect2.x + _rect2.width / 2 };
	int rect2CenterY{ _rect2.y + _rect2.height / 2 };


	int rangeX{ (_rect1.width + _rect2.width) / 2 };
	int rangeY{ (_rect1.height + _rect2.height) / 2 };
	int distanceX{ std::abs(rect1CenterX - rect2CenterX) };
	int distanceY{ std::abs(rect1CenterY - rect2CenterY) };

	return
	{
		distanceX <= rangeX
		&&
		distanceY <= rangeY
	};
}

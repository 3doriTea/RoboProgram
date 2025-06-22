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
	int rangeX{ (_rect1.width + _rect2.width) / 2 };
	int rangeY{ (_rect1.height + _rect2.height) / 2 };
	int distanceX{ std::abs(_rect1.x - _rect2.x) };
	int distanceY{ std::abs(_rect1.y - _rect2.y) };

	return
	{
		distanceX <= rangeX
		&&
		distanceY <= rangeY
	};
}

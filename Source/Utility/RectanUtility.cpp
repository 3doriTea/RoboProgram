#include "RectanUtility.h"

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

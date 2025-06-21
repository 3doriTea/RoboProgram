#pragma once
#include "Vector2.h"


struct Rectan
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		Vector2 pivot;
	};
	union
	{
		struct
		{
			float width;
			float height;
		};
		Vector2 size;
	};

	Vector2 GetCenter() const
	{
		return { pivot.x + width / 2, pivot.y + height / 2 };
	}

	Vector2 GetEnd() const
	{
		return { pivot.x + width, pivot.y + height };
	}
};

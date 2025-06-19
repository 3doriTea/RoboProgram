#pragma once
#include "Vector2Int.h"

struct RectInt
{
	union
	{
		struct
		{
			int x;
			int y;
		};
		Vector2Int pivot;
	};
	union
	{
		struct
		{
			int width;
			int height;
		};
		Vector2Int size;
	};
};

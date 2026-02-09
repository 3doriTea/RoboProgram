#pragma once
#include "Vector2Int.h"


/// <summary>
/// ®”Œ^ ‹éŒ`\‘¢‘Ì
/// </summary>
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

#pragma once

struct Vector2Int
{
	Vector2Int(const int _x, const int _y) :
		x{ _x },
		y{ _y }
	{}
	Vector2Int() :
		x{ 0 },
		y{ 0 }
	{}

	int x;
	int y;

	static inline Vector2Int Zero() { return{ 0, 0 }; }
};

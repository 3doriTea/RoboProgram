#pragma once
#include "Vector2.h"

struct Vector2;

struct Vector2Int
{
	Vector2Int(const Vector2& _other) :
		Vector2Int{ _other.x, _other.y }
	{}

	Vector2Int(const int _x, const int _y) :
		x{ _x },
		y{ _y }
	{}
	Vector2Int() :
		x{ 0 },
		y{ 0 }
	{}

	/*Vector2Int& operator=(const Vector2& _other)
	{
		x = _other.x;
		y = _other.y;
		return *this;
	}*/

	int x;
	int y;

	inline Vector2Int& operator/=(const int _v)
	{
		x /= _v;
		y /= _v;
		return *this;
	}

	inline Vector2Int& operator*=(const int _v)
	{
		x *= _v;
		y *= _v;
		return *this;
	}

	static inline Vector2Int Zero() { return{ 0, 0 }; }
};

inline Vector2Int operator/(const Vector2Int& _v1, const int _val)
{
	return Vector2Int{ _v1 } /= _val;
}

inline Vector2Int operator/(const int _val, const Vector2Int& _v1)
{
	return Vector2Int{ _v1 } /= _val;
}

inline Vector2Int operator*(const Vector2Int& _v1, const int _val)
{
	return Vector2Int{ _v1 } *= _val;
}

inline Vector2Int operator*(const int _val, const Vector2Int& _v1)
{
	return Vector2Int{ _v1 } *= _val;
}

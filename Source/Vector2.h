#pragma once

struct Vector2
{
	Vector2() :
		Vector2{ 0, 0 }
	{}

	Vector2(const float _x, const float _y) :
		x{ _x },
		y{ _y }
	{}

	float x;
	float y;

	inline Vector2& operator+=(const Vector2& _v)
	{
		x += _v.x;
		y += _v.y;
		return *this;
	}
};

inline Vector2 operator+(const Vector2& _v1, const Vector2& _v2)
{
	return Vector2{ _v1 } += _v2;
}

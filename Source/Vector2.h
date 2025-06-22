#pragma once
#include "Vector2Int.h"
#include <cmath>


struct Vector2Int;

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

	Vector2& operator=(const Vector2Int& _other)
	{
		x = static_cast<float>(_other.x);
		y = static_cast<float>(_other.y);
		return *this;
	}

	inline Vector2& operator+=(const Vector2& _v)
	{
		x += _v.x;
		y += _v.y;
		return *this;
	}
	inline Vector2& operator-=(const Vector2& _v)
	{
		x -= _v.x;
		y -= _v.y;
		return *this;
	}

	inline float Size() const
	{
		return std::sqrtf(x * x + y * y);
	}

	/// <summary>
	/// êÆêîå^Ç…ïœä∑Ç∑ÇÈ
	/// </summary>
	/// <returns></returns>
	inline Vector2Int ToInt() const { return { static_cast<int>(x), static_cast<int>(y) }; }
};

inline Vector2 operator+(const Vector2& _v1, const Vector2& _v2)
{
	return Vector2{ _v1 } += _v2;
}

inline Vector2 operator-(const Vector2& _v1, const Vector2& _v2)
{
	return Vector2{ _v1 } -= _v2;
}

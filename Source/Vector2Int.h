#pragma once


/// <summary>
/// ®”Œ^2ŸŒ³ƒxƒNƒgƒ‹
/// </summary>
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
	inline Vector2Int& operator+=(const Vector2Int& _other)
	{
		x += _other.x;
		y += _other.y;
		return *this;
	}

	inline Vector2Int& operator-=(const Vector2Int& _other)
	{
		x -= _other.x;
		y -= _other.y;
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

inline Vector2Int operator+(const Vector2Int& _v1, const Vector2Int& _v2)
{
	return Vector2Int{ _v1 } += _v2;
}

inline Vector2Int operator-(const Vector2Int& _v1, const Vector2Int& _v2)
{
	return Vector2Int{ _v1 } -= _v2;
}

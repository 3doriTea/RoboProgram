#pragma once
#include "../Rectan.h"


namespace RectanUtility
{
	/// <summary>
	/// 矩形内に点が当たっているか
	/// </summary>
	/// <param name="_rect">矩形</param>
	/// <param name="_p">点</param>
	/// <returns>当たっている true / false</returns>
	bool IsHit(const Rectan& _rect, const Vector2& _p);

	/// <summary>
	/// 整数型矩形同士が当たっているか
	/// </summary>
	/// <param name="_rect1">整数型矩形1</param>
	/// <param name="_rect2">整数型矩形1</param>
	/// <returns>当っている true / false</returns>
	bool IsHit(const RectInt& _rect1, const RectInt& _rect2);
}

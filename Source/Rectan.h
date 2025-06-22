#pragma once
#include "Vector2.h"
#include "RectInt.h"

/// <summary>
/// <para>矩形は以下の要素で成り立つ</para>
/// <para>・中心軸となる座標(x,y = pivot)</para>
/// <para>・中心軸からのサイズ(width,height = size)</para>
/// </summary>
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

	RectInt ToInt() const
	{
		RectInt rectInt{};

		rectInt.pivot = pivot.ToInt();
		rectInt.size = size.ToInt();

		return rectInt;
	}

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetCenter() const
	{
		return { pivot.x + width / 2, pivot.y + height / 2 };
	}

	/// <summary>
	/// 終点座標を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetEnd() const
	{
		return { pivot.x + width, pivot.y + height };
	}

	/// <summary>
	/// 最大の頂点座標を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetMax() const
	{
		return
		{
			width > 0 ? (x + width) : (x),
			height > 0 ? (y + height) : (y),
		};
	};

	/// <summary>
	/// 最小の頂点座標を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetMin() const
	{
		return
		{
			width < 0 ? (x - width) : (x),
			height < 0 ? (y - height) : (y),
		};
	};
};

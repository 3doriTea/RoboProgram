#pragma once
#include "Vector2.h"
#include "RectInt.h"

/// <summary>
/// <para>��`�͈ȉ��̗v�f�Ő��藧��</para>
/// <para>�E���S���ƂȂ���W(x,y = pivot)</para>
/// <para>�E���S������̃T�C�Y(width,height = size)</para>
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
	/// ���S���W���擾
	/// </summary>
	/// <returns></returns>
	Vector2 GetCenter() const
	{
		return { pivot.x + width / 2, pivot.y + height / 2 };
	}

	/// <summary>
	/// �I�_���W���擾
	/// </summary>
	/// <returns></returns>
	Vector2 GetEnd() const
	{
		return { pivot.x + width, pivot.y + height };
	}

	/// <summary>
	/// �ő�̒��_���W���擾
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
	/// �ŏ��̒��_���W���擾
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

#pragma once
#include "../Rectan.h"


namespace RectanUtility
{
	/// <summary>
	/// ��`���ɓ_���������Ă��邩
	/// </summary>
	/// <param name="_rect">��`</param>
	/// <param name="_p">�_</param>
	/// <returns>�������Ă��� true / false</returns>
	bool IsHit(const Rectan& _rect, const Vector2& _p);

	/// <summary>
	/// �����^��`���m���������Ă��邩
	/// </summary>
	/// <param name="_rect1">�����^��`1</param>
	/// <param name="_rect2">�����^��`1</param>
	/// <returns>�����Ă��� true / false</returns>
	bool IsHit(const RectInt& _rect1, const RectInt& _rect2);
}

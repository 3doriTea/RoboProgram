#pragma once
#include <vector>


namespace VectorUtil
{
	/// <summary>
	/// <para>std::vector�̃R�s�[������</para>
	/// <para>TODO: �����R�s�[����������</para>
	/// </summary>
	/// <typeparam name="T">�v�f�̌^</typeparam>
	/// <param name="_dest">�R�s�[��</param>
	/// <param name="_src">�R�s�[��</param>
	template<typename T>
	void CopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	{
		_dest = _src;
	}
}

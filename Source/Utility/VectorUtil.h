#pragma once
#include <vector>


namespace VectorUtil
{
	/// <summary>
	/// <para>std::vectorのコピーをする</para>
	/// <para>TODO: 早いコピーを実装する</para>
	/// </summary>
	/// <typeparam name="T">要素の型</typeparam>
	/// <param name="_dest">コピー先</param>
	/// <param name="_src">コピー元</param>
	template<typename T>
	void CopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	{
		_dest = _src;
	}
}

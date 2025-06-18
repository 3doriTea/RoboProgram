#pragma once
#include <vector>


namespace VectorUtil
{
	/// <summary>
	/// 速いstd::vector<T>のコピー
	/// </summary>
	/// <typeparam name="T">要素の型</typeparam>
	/// <param name="_dest">コピー先</param>
	/// <param name="_src">コピー元</param>
	//template<typename T>
	//void FirstCopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	//{
	//	if (_dest.size() != _src.size())
	//	{
	//		_dest.clear();
	//		_dest.resize(_src.size());  // サイズが違うなら合わせる
	//	}

	//	std::memcpy(_dest.data(), _src.data(), _dest.size() * sizeof(T));
	//}

	template<typename T>
	void CopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	{
		_dest = _src;
	}
}

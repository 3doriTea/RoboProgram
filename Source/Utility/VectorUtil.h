#pragma once
#include <vector>


namespace VectorUtil
{
	/// <summary>
	/// ����std::vector<T>�̃R�s�[
	/// </summary>
	/// <typeparam name="T">�v�f�̌^</typeparam>
	/// <param name="_dest">�R�s�[��</param>
	/// <param name="_src">�R�s�[��</param>
	//template<typename T>
	//void FirstCopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	//{
	//	if (_dest.size() != _src.size())
	//	{
	//		_dest.clear();
	//		_dest.resize(_src.size());  // �T�C�Y���Ⴄ�Ȃ獇�킹��
	//	}

	//	std::memcpy(_dest.data(), _src.data(), _dest.size() * sizeof(T));
	//}

	template<typename T>
	void CopyVector(std::vector<T>& _dest, const std::vector<T> _src)
	{
		_dest = _src;
	}
}

#pragma once
#include <string>


namespace StringUtility
{
	/// <summary>
	/// wstring��string�ɕϊ�����
	/// </summary>
	/// <param name="_wstr">���C�h������</param>
	/// <returns>�����̕�����</returns>
	std::string ToString(const std::wstring& _wstr);
}

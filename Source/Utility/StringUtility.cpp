#include "StringUtility.h"

std::string StringUtility::ToString(const std::wstring& _wstr)
{
	return { _wstr.begin(), _wstr.end() };
}

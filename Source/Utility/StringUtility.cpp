#include "StringUtility.h"

#pragma warning(disable:4244)

std::string StringUtility::ToString(const std::wstring& _wstr)
{
	return { _wstr.begin(), _wstr.end() };
}

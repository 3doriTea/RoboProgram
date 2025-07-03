#pragma once
#include <string>
#include "Analyzer.h"

namespace Assembler
{
	void ToAssemble(const ByteCodes& bc, std::string& outStr);
}

#pragma once
#include <string>
#include "Analyzer.h"

namespace Assembler
{
	/// <summary>
	/// �o�C�g�R�[�h�����̂܂܂ƃA�Z���u�����ȕ�����ɕϊ�����
	/// </summary>
	/// <param name="bc">����:�o�C�g�R�[�h</param>
	/// <param name="outStr">�o��:������̎Q�ƃ|�C���^</param>
	void ToString(const ByteCodes& bc, std::string& outStr);
}

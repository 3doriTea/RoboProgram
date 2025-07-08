#pragma once
#include <string>
#include "Analyzer.h"

namespace Assembler
{
	/// <summary>
	/// バイトコードをそのままとアセンブリ風な文字列に変換する
	/// </summary>
	/// <param name="bc">入力:バイトコード</param>
	/// <param name="outStr">出力:文字列の参照ポインタ</param>
	void ToString(const ByteCodes& bc, std::string& outStr);
}

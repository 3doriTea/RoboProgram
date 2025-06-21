#include "ProtoAnalyzer.h"
#include <sstream>
#include "../ByteCodeDefine.h"


ProtoAnalyzer::~ProtoAnalyzer()
{
}

void ProtoAnalyzer::Analyze(std::vector<std::pair<int, Byte>>& _outRef)
{
	_outRef.clear();

	for (int line = 0; line < in_.size(); line++)
	{
		const std::string& lineStr{ in_[line] };
		std::string word{};
		bool isInComment{ false };

		for (auto itr = lineStr.begin(); itr != lineStr.end(); itr++)
		{
			if ((*itr) == '/')
			{
				itr++;
				if (itr == lineStr.end())
				{
					break;
				}
				if ((*itr) == '/')
				{
					// コメント判定
					isInComment = true;
					break;
				}
			}

			if (isInComment)
			{
				break;  // コメントならこの行無視
			}

			if ((*itr) == ' '
				|| (*itr) == '\t')
			{
				continue;  // 空白系は無視
			}

			word += (*itr);
		}

		if (word == "Run();")
		{
			_outRef.push_back({ line, BCD_ACT });
			_outRef.push_back({ line, BCD_ACT_RUN });
		}
		else if (word == "Jump();")
		{
			_outRef.push_back({ line, BCD_ACT });
			_outRef.push_back({ line, BCD_ACT_JUMP });
		}
		else
		{
			_outRef.push_back({ line, BCD_NOP });
		}
	}
}

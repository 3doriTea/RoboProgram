#include "ProtoAnalyzer.h"
#include <sstream>

ProtoAnalyzer::~ProtoAnalyzer()
{
}

void ProtoAnalyzer::Analyze(std::vector<std::pair<int, Byte>>& _outRef)
{
	_outRef.clear();

	for (const auto& line : in_)
	{
		std::string word{};
		bool isInComment{ false };

		for (auto itr = line.begin(); itr != line.end(); itr++)
		{
			if ((*itr) == '/')
			{
				itr++;
				if (itr == line.end())
				{
					break;
				}
				if ((*itr) == '/')
				{
					// �R�����g����
					isInComment = true;
					break;
				}
			}

			if (isInComment)
			{
				break;
			}

			if ((*itr) == ' '
				|| (*itr) == '\t')
			{
				break;  // �󔒌n�͖���
			}

			word += (*itr);
		}

		if (word == "Run();")
		{

		}
	}
}

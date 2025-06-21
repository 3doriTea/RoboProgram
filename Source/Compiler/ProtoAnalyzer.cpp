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
					// ƒRƒƒ“ƒg”»’è
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
				break;  // ‹ó”’Œn‚Í–³‹
			}

			word += (*itr);
		}

		if (word == "Run();")
		{

		}
	}
}

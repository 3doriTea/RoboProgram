#include "LexicalAnalyzer.h"


void LexicalAnalyzer::Analyze(Tokens& _outRef)
{
	for (const auto& line : in_)
	{
		std::string token{};

		auto ConsumePush = [&, token]()
		{
		};

		for (int column = 0; column < line.second.size(); column++)
		{
			token[0]
		}
		//line.second
	}
}

#include "SyntaxAnalyzer.h"

void SyntaxAnalyzer::Analyze()
{
	for (readIndex_ = 0; readIndex_ < in_.size(); readIndex_++)
	{
		
	}
}

void SyntaxAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, in_[readIndex_].first);
}

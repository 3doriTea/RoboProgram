#pragma once
#include "Analyzer.h"

/// <summary>
/// š‹å‰ğÍ
/// </summary>
class LexicalAnalyzer : public Analyzer<SourceLines, Tokens>
{
public:
	using Analyzer::Analyzer;
	void Analyze(Tokens& _outRef);
};

#pragma once
#include "Analyzer.h"

/// <summary>
/// ������
/// </summary>
class LexicalAnalyzer : public Analyzer<SourceLines, Tokens>
{
public:
	using Analyzer::Analyzer;
	void Analyze(Tokens& _outRef);
};

#pragma once
#include "Analyzer.h"
#include <vector>
#include <string>
#include "../CodeReader/ByteCodeReader.h"


/// <summary>
/// �ȈՓI�ȉ�͋@
/// </summary>
class ProtoAnalyzer : public Analyzer<
	ProtoAnalyzer,
	std::vector<std::string>,
	std::vector<std::pair<int, Byte>>,
	LineCount>
{
public:
	using Analyzer::Analyzer;
	~ProtoAnalyzer();

	void Analyze() override;

private:
	ProtoAnalyzer& Get() override { return *this; }
};

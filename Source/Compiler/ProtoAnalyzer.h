#pragma once
#include "Analyzer.h"
#include <vector>
#include <string>
#include "../CodeReader/ByteCodeReader.h"


/// <summary>
/// �ȈՓI�ȉ�͋@
/// </summary>
class ProtoAnalyzer : public Analyzer<
	std::vector<std::string>,
	std::vector<std::pair<int, Byte>>>
{
public:
	using Analyzer::Analyzer;
	~ProtoAnalyzer();

	void Analyze(std::vector<std::pair<int, Byte>>& _outRef) override;
};

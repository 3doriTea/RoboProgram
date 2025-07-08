#pragma once
#include "Analyzer.h"
#include <vector>
#include <string>
#include "../CodeReader/ByteCodeReader.h"


/// <summary>
/// 簡易的な解析機
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
	/// <summary>
	/// 基底クラスに渡す用
	/// </summary>
	/// <returns></returns>
	ProtoAnalyzer& Get() override { return *this; }
};

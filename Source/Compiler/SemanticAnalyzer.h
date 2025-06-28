#pragma once
#include "Analyzer.h"

class SemanticAnalyzer : public Analyzer<
	SemanticAnalyzer,
	Nodes,
	Nodes,
	SOURCE_POS>
{
public:
	using Analyzer::Analyzer;
	/// <summary>
	/// ��͂���
	/// </summary>
	void Analyze() override;

private:
	SemanticAnalyzer& Get() { return *this; }
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const char* _message) override;
};


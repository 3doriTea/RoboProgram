#pragma once
#include "Analyzer.h"

class SemanticAnalyzer : public Analyzer<
	SemanticAnalyzer,
	std::pair<Nodes&, Tokens&>,
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
	void Read(const NODE* n, const int _depth);

	SemanticAnalyzer& Get() { return *this; }
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const char* _message) override;
};


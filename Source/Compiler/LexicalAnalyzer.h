#pragma once
#include "Analyzer.h"

/// <summary>
/// ������
/// </summary>
class LexicalAnalyzer : public Analyzer<
	LexicalAnalyzer,
	SourceLines,
	Tokens,
	SOURCE_POS>
{
public:
	using Analyzer::Analyzer;
	void Analyze() override;

private:
	/// <summary>
	/// ���N���X�ɓn���p
	/// </summary>
	/// <returns></returns>
	LexicalAnalyzer& Get() override { return *this; };

	void Error(const char* _message) override;
	/// <summary>
	/// ������g�[�N�����m�肳���|�b�v&�N���A����
	/// </summary>
	/// <param name="_token">�m�肳����g�[�N��</param>
	void PopToken(std::string& _token);
	/// <summary>
	/// �����P�̂��g�[�N���Ƃ��ă|�b�v
	/// </summary>
	/// <param name="_token"></param>
	void PopToken(const char _token);

private:
	LineCount lineCount_;
	ColumnCount columnCount_;
};

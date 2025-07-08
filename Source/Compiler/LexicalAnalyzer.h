#pragma once
#include "Analyzer.h"

/// <summary>
/// 字句解析
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
	/// 基底クラスに渡す用
	/// </summary>
	/// <returns></returns>
	LexicalAnalyzer& Get() override { return *this; };

	void Error(const char* _message) override;
	/// <summary>
	/// 文字列トークンを確定させポップ&クリアする
	/// </summary>
	/// <param name="_token">確定させるトークン</param>
	void PopToken(std::string& _token);
	/// <summary>
	/// 文字単体をトークンとしてポップ
	/// </summary>
	/// <param name="_token"></param>
	void PopToken(const char _token);

private:
	LineCount lineCount_;
	ColumnCount columnCount_;
};

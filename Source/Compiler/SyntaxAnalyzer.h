#pragma once
#include "Analyzer.h"
#include <map>
#include <stack>


///// <summary>
///// 型情報
///// </summary>
//struct TypeInfo
//{
//	Byte size;  // 型のサイズ
//	const TokenString& name;  // 型名
//};

/// <summary>
/// 構文解析するクラス
/// </summary>
class SyntaxAnalyzer : public Analyzer<
	SyntaxAnalyzer,
	Tokens,
	Nodes,
	SOURCE_POS>
{
public:
	using Analyzer::Analyzer;
	/// <summary>
	/// 解析する
	/// </summary>
	void Analyze() override;

private:
#pragma region スタック解決する関数
	NODE* Global();
	NODE* FuncDef();
#pragma endregion


#pragma region 消費とか
	bool Consume(const std::string& _str);
	bool Except(const std::string& _str);
#pragma endregion

	SyntaxAnalyzer& Get() { return *this; }
	/// <summary>
	/// エラー処理
	/// </summary>
	/// <param name="_message">エラーメッセージ</param>
	void Error(const char* _message) override;

private:
	int readIndex_{};
	std::map<TokenString, Byte> typeRegister_{};
	std::map<TokenString, Byte> funcRegister_{};
	std::stack<std::map<TokenString, Byte>> varRegister_{};
};

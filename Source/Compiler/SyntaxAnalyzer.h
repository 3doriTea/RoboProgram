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
	NODE* _Expr();
	NODE* _Lor();
	NODE* _Land();
	NODE* _Equal();
	NODE* _Then();
	NODE* _Add();
	NODE* _Mul();
	NODE* _Unary();
	NODE* _Postfix();
	NODE* _Primary();

	NODE* _Val();
	NODE* _Liter();
	NODE* _Var();
	NODE* _CallFunc();

	NODE* _Name();
	NODE* _Type();
	NODE* _Integer();
#pragma endregion


#pragma region 消費とか
	NODE* NewNode(const NODE& _node);
	/// <summary>
	/// 一致していたら進める
	/// </summary>
	/// <param name="_str">比較文字列</param>
	/// <returns>一致している true / false</returns>
	bool Consume(const std::string& _str);
	/// <summary>
	/// 取って捨てる
	/// </summary>
	/// <param name="_str"></param>
	void Expect(const std::string& _str);
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

#pragma once
#include "Analyzer.h"
#include <map>
#include <stack>


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
	NODE* _Global();

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

	NODE* _Args();
	
	NODE* _NFor();
	NODE* _NIf();
	NODE* _Block();

	NODE* Procs(int _callCount = 0);
	NODE* Proc();

	NODE* _Func();

	NODE* _Return();

	NODE* _Params();

	NODE* _FuncDef();

	NODE* _Name();
	NODE* _Type();
	NODE* _Integer();

	NODE* _VarDec();
	NODE* _Assign();
#pragma endregion


#pragma region 消費とか

	/// <summary>
	/// 読み取り不可能か
	/// </summary>
	/// <param name="_size">要求サイズ</param>
	/// <returns>読み取り不可能 true / false</returns>
	bool IsUnreadable(const size_t _size);

	/// <summary>
	/// 次に進める
	/// </summary>
	/// <returns>現在のインデックス</returns>
	int Advance();

	/// <summary>
	/// 覗き見する
	/// </summary>
	/// <param name="_offset">読み取り位置からのオフセット</param>
	/// <returns>トークンのreadonly</returns>
	const std::string& Peek(const int _offset = 0);
	/// <summary>
	/// 新しいノードを作る
	/// </summary>
	/// <param name="_node">ノードの情報(コピー代入される)</param>
	/// <returns>作られたノードのポインタ</returns>
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

	/// <summary>
	/// 現在のインデックスを取得
	/// </summary>
	/// <returns></returns>
	inline const int GetIdx() const { return readIndex_ - 1; }
#pragma endregion
	/// <summary>
	/// 基底クラスに渡す用
	/// </summary>
	/// <returns></returns>
	SyntaxAnalyzer& Get() override { return *this; }
	/// <summary>
	/// エラー処理
	/// </summary>
	/// <param name="_message">エラーメッセージ</param>
	void Error(const char* _message) override;

private:
	int readIndex_{};  // 見ているトークンのインデックス
	std::map<TokenString, Byte> typeRegister_{};  // 使用型の登録
	std::map<TokenString, Byte> funcRegister_{};  // 使用関数の登録
	std::stack<std::map<TokenString, Byte>> varRegister_{};  // 使用変数の登録
};

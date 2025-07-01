#pragma once
#include "Analyzer.h"
#include <map>
#include <string>

class SemanticAnalyzer : public Analyzer<
	SemanticAnalyzer,
	std::pair<Nodes&, Tokens&>,
	ByteCodes,
	SOURCE_POS>
{
private:
	struct FuncData
	{
		FuncData() :
			index{ -1 },
			byteCodes{}
		{}

		int index;
		ByteCodes byteCodes;
	};

	struct UseMemory
	{
		UseMemory() :
			UseMemory{ -1, 0 }
		{}
		UseMemory(const int _offset, const int _size) :
			offset{ _offset },
			size{ _size }
		{}

		int offset;        // 仕様メモリ位置
		int size;          // 使用メモリサイズ
	};

	struct TypeInfo
	{
		enum Type
		{
			INT,
			BOOL,
		};
	};
public:
	using Analyzer::Analyzer;
	/// <summary>
	/// 解析する
	/// </summary>
	void Analyze() override;

private:
	/// <summary>
	/// メモリ領域を確保する
	/// </summary>
	/// <param name="name">空間の名前</param>
	/// <param name="size">要求サイズ</param>
	/// <returns>確保できたメモリの番地</returns>
	int NewMemory(const std::string name, int size);

	/// <summary>
	/// 付けた名前からメモリの領域を取得する
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	int GetMemory(const std::string& name);

	void Read(const NODE* n, const int _depth);
	void ReadFuncDec(const NODE* n);
	std::string ReadName(const NODE* n);
	std::string ReadType(const NODE* n);
	int ReadIntegier(const NODE* n);
	void ReadParam(const NODE* n, ByteCodes& bc);
	void ReadProcs(const NODE* n, ByteCodes& bc);

	void ReadExpr(const NODE* n, ByteCodes& bc);

	/// <summary>
	/// レジスタからメモリに値をセットする
	/// </summary>
	/// <param name="_addr"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void MemorySet(const int _addr, const int _size, ByteCodes& bc);

	/// <summary>
	/// メモリからレジスタに値をセットする
	/// </summary>
	/// <param name="_addr"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void MemoryGet(const int _addr, const int _size, ByteCodes& bc);


	SemanticAnalyzer& Get() { return *this; }
	/// <summary>
	/// エラー処理
	/// </summary>
	/// <param name="_message">エラーメッセージ</param>
	void Error(const char* _message) override;


	int maxMemorySize{};
	std::map<std::string, UseMemory> memory{};
	std::map<std::string, FuncData> funcGroup{};
};

#pragma once
#include <vector>
#include <string>
#include <functional>


using Byte = unsigned char;

using LineCount = int;
using LineString = std::string;

using ColumnCount = int;

/// <summary>
/// ソースコードの座標情報
/// </summary>
struct SOURCE_POS
{
	SOURCE_POS() :
		SOURCE_POS{ -1, -1 }
	{}
	SOURCE_POS(
		const LineCount _line,
		const ColumnCount _column) :
		line{ _line },
		column{ _column }
	{}
	LineCount line;
	ColumnCount column;
};

// ソースコードの1行分
using SourceLine = std::pair<LineCount, LineString>;
using SourceLines = std::vector<SourceLine>;

using TokenString = std::string;

// トークン
using Token = std::pair<SOURCE_POS, TokenString>;
using Tokens = std::vector<Token>;

// モードの型
enum NodeType
{
	NODE_GLOBAL,

	NODE_PROC,  // 処理
	NODE_EXPR,  // 式
	NODE_OR,  // ||
	NODE_AND,  // &&
	NODE_EQUAL,  // ==
	NODE_NOTEQUAL,  // !=
	NODE_LESSTHAN,  // <
	NODE_GREATERTHEN,  // >
	NODE_LESSEQUAL,  // <=
	NODE_GREATEREQUAL,  // >=
	NODE_ADD,  // +
	NODE_SUB,  // -
	NODE_MUL,  // *
	NODE_DIV,  // /
	NODE_NFOR,  // for文
	NODE_NIF,   // if文
	NODE_VARDEC,  // 変数宣言
	NODE_FUNCDEC,  // 関数宣言
	//NODE_VALUE,  // 値
	NODE_INTEGER,  // 整数値

	//NODE_LITER_DIGIT,  // 整数リテラル

	NODE_CALLFUNC,  // 関数呼び出し
	NODE_INCREMENT,  // ++
	NODE_DECREMENT,  // --

	NODE_NAME,  // 名
	NODE_TYPE,  // 型

	NODE_RET,  // return

	NODE_ARG,  // 実引数

	NODE_ASSIGN,  // 代入式

	NODE_PARAM,  // 仮引数

	//NODE_NUMBER,             // 数字
	NODE_REGISTER_FUNC_NAME, // 登録されている関数
	NODE_REGISTER_VAR_NAME,  // (登録されている変数名)
	//NODE_BOOL,               // true | false
	//NODE_REGISTER_TYPE_NAME, // (登録されている型名)
};

// ノード
struct NODE
{  // total 32byte
	NODE(const int _tokenIndex, const NodeType _type) :
		NODE{ _tokenIndex, _type, nullptr, nullptr, nullptr, nullptr }
	{}
	NODE(const int _tokenIndex, const NodeType _type, NODE* _n1) :
		NODE{ _tokenIndex, _type, _n1, nullptr, nullptr, nullptr }
	{}
	NODE(const int _tokenIndex, const NodeType _type, NODE* _n1, NODE* _n2) :
		NODE{ _tokenIndex, _type, _n1, _n2, nullptr, nullptr }
	{}
	NODE(const int _tokenIndex, const NodeType _type, NODE* _n1, NODE* _n2, NODE* _n3) :
		NODE{ _tokenIndex, _type, _n1, _n2, _n3, nullptr }
	{}
	NODE(const int _tokenIndex, const NodeType _type, NODE* _n1, NODE* _n2, NODE* _n3, NODE* _n4) :
		tokenIndex_{ _tokenIndex },
		type_{ _type },
		node1{ _n1 },
		node2{ _n2 },
		node3{ _n3 },
		node4{ _n4 }
	{}

	int tokenIndex_;  // トークンのインデックス
	NodeType type_;  // ノードの種類
	
	union  // 32byte
	{
		struct
		{
			NODE* node1;
			NODE* node2;
			NODE* node3;
			NODE* node4;
		};

		union  // 32byte
		{
			struct  // 関数宣言
			{
				NODE* type;
				NODE* name;
				NODE* param;
				NODE* proc;  // 処理
			} funcDec;

			struct  // 反復処理
			{
				NODE* init;  // assign 初期化処理
				NODE* expr;  // expr 継続条件
				NODE* updt;  // expr 更新処理
				NODE* proc;  // proc 処理
			} nfor;
		};

		union  // 24byte
		{
			struct  // 代入式
			{
				NODE* name;
				NODE* expr;
				NODE* next;
			} assigns;
		};

		union  // 16byte
		{
			struct  // 式
			{
				NODE* ls;  // left side
				NODE* rs;  // right side
			} expr;

			struct  // グローバル空間
			{
				NODE* funcDef;
				NODE* next;
			} global;

			struct  // 変数宣言
			{
				NODE* type;
				NODE* assigns;
			} varDec;

			struct  // 関数呼出し
			{
				NODE* name;
				NODE* args;
			} callFunc;

			struct  // 処理
			{
				NODE* proc;
				NODE* next;
			} proc;

			struct  // 実引数
			{
				NODE* expr;
				NODE* next;
			} arg;

			struct  // 仮引数
			{
				NODE* varDec;
				NODE* next;
			} param;

			struct  // 条件分岐
			{
				NODE* expr;
				NODE* proc;
			} nif;
		};

		union  // 8byte
		{
			struct  // 返却文
			{
				NODE* expr;
			} ret;

			struct  // 登録済み関数名
			{
				NODE* name;
			} func;

			struct  // 変数
			{
				NODE* name;
			} var;

			struct  // リテラル
			{
				NODE* liter;
			} liter;

			struct  // インクリメント
			{
				NODE* expr;
			} increment;

			struct  // デクリメント
			{
				NODE* expr;
			} decrement;
		};
	};
};

// 構文ノード
using Nodes = std::vector<NODE*>;

// バイトコード
using ByteCode = std::pair<SOURCE_POS, Byte>;

struct ByteCodes : public std::vector<ByteCode>
{
	using std::vector<ByteCode>::vector;
	int offset;  // バイトコード群の絶対オフセット
};

//ByteCodes;

/// <summary>
/// 解析機
/// </summary>
/// <typeparam name="SubAnalyzerT">派生させた解析クラスの型</typeparam>
/// <typeparam name="InT">解析するオブジェクトの型</typeparam>
/// <typeparam name="OutT">成果物とされるオブジェクトの型</typeparam>
/// <typeparam name="ErrorArgT">エラー時の引数</typeparam>
template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
class Analyzer
{
public:
	Analyzer(const InT& _inRef, OutT& out_);
	virtual ~Analyzer();

	/// <summary>
	/// 解析する
	/// </summary>
	/// <param name="_outRef">成果物が入る参照ポインタ</param>
	virtual void Analyze() = 0;

	inline SubAnalyzerT& OnError(const std::function<void(const char* _message, const ErrorArgT& _arg)>& _onError)
	{
		onError_ = _onError;
		return Get();
	}

protected:
	virtual SubAnalyzerT& Get() = 0;

	/// <summary>
	/// エラー処理
	/// </summary>
	/// <param name="_message">エラーメッセージ</param>
	/// <param name="_arg">エラー引数</param>
	inline void ErrorFull(const char* _message, const ErrorArgT& _arg)
	{
		onError_(_message, _arg);
	}

	/// <summary>
	/// エラー処理(簡易版を派生先で用意させる)
	/// </summary>
	/// <param name="_message">エラーメッセージ</param>
	inline virtual void Error(const char* _message) {};

protected:
	const InT& in_;
	OutT& out_;

private:
	std::function<void(const char* _message, const ErrorArgT& _arg)> onError_;
};

template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
inline Analyzer<SubAnalyzerT, InT, OutT, ErrorArgT>::Analyzer(const InT& _inRef, OutT& _outRef) :
	in_{ _inRef },
	out_{ _outRef }
{
}

template<typename SubAnalyzerT, typename InT, typename OutT, typename ErrorArgT>
inline Analyzer<SubAnalyzerT, InT, OutT, ErrorArgT>::~Analyzer()
{
}

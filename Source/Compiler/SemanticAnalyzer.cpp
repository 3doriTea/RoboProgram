#include "SemanticAnalyzer.h"
#include <cassert>
#include <DxLib.h>
#include "../ByteCodeDefine.h"

static const size_t DEBUG_STR_SIZE{ 64 };

char debugString[DEBUG_STR_SIZE]{};

#define PRINTF(...) \
::sprintf_s<64>(debugString, __VA_ARGS__); \
OutputDebugString(debugString);

void SemanticAnalyzer::Analyze()
{
	if (in_.first.size() <= 0)
	{
		Error("トークン木がないよ！");
		return;
	}
	for (auto& nodes : in_.first)
	{
		if (nodes->type_ == NODE_GLOBAL)
		{
			Read(nodes, 0);
			return;
		}
	}

	Error("グローバルがないよ！");
}

int SemanticAnalyzer::NewMemory(std::string name, int size)
{
	assert(size == 4 && "4byte以外の型サイズは未実装です。");

	std::vector<bool> used(maxMemorySize / 4, false);

	int offset = 0;
	for (auto& mem : memory)
	{
		used[mem.second.offset / 4] = true;
		offset++;
	}

	for (int i = 0; i < used.size(); i++)
	{
		// 使われていない場所発見
		if (used[i] == false)
		{
			memory.insert({ name, UseMemory{ offset * 4, 4 }});
			return i * 4;
		}
	}
	// もし空き場所ないなら

	memory.insert({ name, UseMemory{ offset * 4, 4 } });
	int add = maxMemorySize;
	maxMemorySize += size;

	return add;
}

int SemanticAnalyzer::GetMemory(const std::string& name)
{
	return memory[name].offset;
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth)
{
	assert(n != nullptr
		&& "どこかでnullった！");

	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		PRINTF("プロセス\n");
		Read(n->proc.proc, d);
		if (n->proc.next != nullptr)  // 次のプロセスがあるなら
		{
			Read(n->proc.next, d);
		}
		break;// 処理
	case NODE_EXPR:
		PRINTF("式\n");
		Read(n->expr.ls, d);
		Read(n->expr.rs, d);
		break;// 式
	case NODE_OR:
		Read(n->expr.rs, d);
		PRINTF("or\n");
		Read(n->expr.ls, d);
		break;// ||
	case NODE_AND:
		Read(n->expr.rs, d);
		PRINTF("and\n");
		Read(n->expr.ls, d);
		break;// &&
	case NODE_EQUAL:
		Read(n->expr.ls, d);
		PRINTF("==\n");
		Read(n->expr.rs, d);
		break;// ==
	case NODE_NOTEQUAL:
		Read(n->expr.ls, d);
		PRINTF("!=\n");
		Read(n->expr.rs, d);
		break;// !=
	case NODE_LESSTHAN:
		Read(n->expr.ls, d);
		PRINTF("<\n");
		Read(n->expr.rs, d);
		break;// <
	case NODE_GREATERTHEN:
		Read(n->expr.ls, d);
		PRINTF(">\n");
		Read(n->expr.rs, d);
		break;// >
	case NODE_LESSEQUAL:
		Read(n->expr.ls, d);
		PRINTF("<=\n");
		Read(n->expr.rs, d);
		break;// <=
	case NODE_GREATEREQUAL:
		Read(n->expr.ls, d);
		PRINTF(">=\n");
		Read(n->expr.rs, d);
		break;// >=
	case NODE_ADD:
		
		Read(n->expr.ls, d);
		PRINTF("+\n");
		Read(n->expr.rs, d);
		break;// +
	case NODE_SUB:
		Read(n->expr.ls, d);
		PRINTF("-\n");
		Read(n->expr.rs, d);
		break;// -
	case NODE_MUL:
		Read(n->expr.ls, d);
		PRINTF("*\n");
		Read(n->expr.rs, d);
		break;// *
	case NODE_DIV:
		Read(n->expr.ls, d);
		PRINTF("/\n");
		Read(n->expr.rs, d);
		break;// /
	case NODE_NFOR:
		PRINTF("for\n");
		Read(n->nfor.init, d);
		Read(n->nfor.expr, d);
		Read(n->nfor.updt, d);
		Read(n->nfor.proc, d);
		break;// for文
	case NODE_NIF:
		PRINTF("if\n");
		Read(n->nif.expr, d);
		Read(n->nif.proc, d);
		break;// if文
	case NODE_VARDEC:
		PRINTF("変数宣言\n");
		Read(n->varDec.type, d);
		Read(n->varDec.assigns, d);
		break;// 変数宣言
	case NODE_FUNCDEC:
		PRINTF("関数定義\n");
		


		Read(n->funcDec.type, d);
		Read(n->funcDec.name, d);
		if (n->funcDec.param != nullptr)  // パラメータがあるなら
		{
			Read(n->funcDec.param, d);
		}
		Read(n->funcDec.proc, d);
		break;// 関数宣言
	//case NODE_VALUE:
		//PRINTF("値:%s\n", in_.second[n->tokenIndex_].second.c_str());
		
		//break;// 値
	case NODE_INTEGER:
		PRINTF("%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 整数値
	//case NODE_LITER_DIGIT:
	//	PRINTF("\n");
	//	break;// 整数リテラル
	case NODE_CALLFUNC:
		PRINTF("関数呼び出し:\n");
		Read(n->callFunc.name, d);
		if (n->callFunc.args != nullptr)  // 引数があるなら
		{
			Read(n->callFunc.args, d);
		}
		break;// 関数呼び出し
	case NODE_INCREMENT:
		PRINTF("インクリメント:\n");
		Read(n->expr.ls, d);
		break;// ++
	case NODE_DECREMENT:
		PRINTF("デクリメント:\n");
		Read(n->expr.ls, d); 
		break;// --
	case NODE_NAME:
		PRINTF("名前:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 名
	case NODE_TYPE:
		PRINTF("型:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 型
	case NODE_RET:
		PRINTF("返却:\n");
		Read(n->ret.expr, d);
		break;// return
	case NODE_ARG:
		PRINTF("実引数:\n");
		Read(n->arg.expr, d);
		if (n->arg.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->arg.next, d);
		}

		break;// 実引数
	case NODE_ASSIGN:
		PRINTF("代入:\n");
		Read(n->assigns.name, d);
		PRINTF("=:\n");
		Read(n->assigns.expr, d);
		if (n->assigns.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->assigns.next, d);
		}
		break;// 代入式
	case NODE_PARAM:
		PRINTF("仮引数\n");
		Read(n->param.varDec, d);
		if (n->param.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->param.next, d);
		}
		break;// 仮引数
	case NODE_REGISTER_FUNC_NAME:
		PRINTF("登録済み関数名:\n");
		Read(n->func.name, d);
		break;
	case NODE_REGISTER_VAR_NAME:
		PRINTF("登録済み変数名:\n");
		Read(n->var.name, d);
		break;
	case NODE_GLOBAL:
		PRINTF("グローバル\n");
		Read(n->global.funcDef, d);
		if (n->global.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->global.next, d);
		}
		break;
	default:
		assert(false && "処理しきれていない");
	}
}

void SemanticAnalyzer::ReadFuncDec(const NODE* n)
{
	std::string funcName = ReadName(n->funcDec.name);
	funcGroup.insert({ funcName, {} });

	FuncData& data{ funcGroup[funcName] };
	ByteCodes& bc{ data.byteCodes };

	//n->funcDec.param
}

std::string SemanticAnalyzer::ReadName(const NODE* n)
{
	assert(n != nullptr && "nullptrだった！ @SemanticAnalyzer::ReadName");
	
	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_が範囲外だった @SemanticAnalyzer::ReadName");
	
	return in_.second[n->tokenIndex_].second;
}

int SemanticAnalyzer::ReadIntegier(const NODE* n)
{
	assert(n != nullptr && "nullptrだった！ @SemanticAnalyzer::ReadName");

	std::string str{ in_.second[n->tokenIndex_].second };

	return std::stoi(str);
}

void SemanticAnalyzer::ReadParam(const NODE* n, ByteCodes& bc)
{
	// TODO: int型だと仮確定している
	// スタックから取り出し、4byte分レジスタに格納する
	bc.push_back({ {}, BCD_POPW });
	bc.push_back({ {}, 4 });


	std::string varName = ReadName(n->param.varDec->varDec.assigns->assigns.name);

	int addr = NewMemory(varName, 4);
	MemorySet(addr, 4, bc);


	//n->param.varDec->varDec.type
}

void SemanticAnalyzer::ReadProcs(const NODE* n, ByteCodes& bc)
{
	//n->
}

void SemanticAnalyzer::ReadExpr(const NODE* n, ByteCodes& bc)
{
	// オペランド(被演算子)だったら
	switch (n->type_)
	{
	case NODE_REGISTER_VAR_NAME:
		ReadVar
		ReadName(n->var.name);
		break;
	case NODE_REGISTER_FUNC_NAME:
		ReadName(n->func.name);
		break;
	case NODE_INTEGER:
		ReadIntegier(n);
		break;
	default:
		break;
	}


	switch (n->type_)
	{
	case NODE_ADD:

		break;
	default:
		assert(false && "計算できない演算子");
		break;
	}
}

void SemanticAnalyzer::MemorySet(const int _addr, const int _size, ByteCodes& bc)
{
	// メモリの _addr からレジスタの _size バイト分を書き込む
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DSET });  // 命令
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // セット先-メモリ
		bc.push_back({ {}, static_cast<Byte>(i) });  // レジスタ指定
	}
}

void SemanticAnalyzer::MemoryGet(const int _addr, const int _size, ByteCodes& bc)
{
	// メモリの _addr からレジスタに _size バイト分を読み込む
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DGET });   // 命令
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // ゲット先-メモリ
		bc.push_back({ {}, static_cast<Byte>(i) });  // レジスタ指定
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, {});
}

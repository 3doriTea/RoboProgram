#include "SemanticAnalyzer.h"
#include <cassert>
#include <DxLib.h>

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
		Read(n->fancDec.type, d);
		Read(n->fancDec.name, d);
		if (n->fancDec.param != nullptr)  // パラメータがあるなら
		{
			Read(n->fancDec.param, d);
		}
		Read(n->fancDec.proc, d);
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

void SemanticAnalyzer::Error(const char* _message)
{
}

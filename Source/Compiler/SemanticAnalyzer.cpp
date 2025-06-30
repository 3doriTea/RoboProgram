#include "SemanticAnalyzer.h"
#include <cassert>
#include <DxLib.h>

void SemanticAnalyzer::Analyze()
{
	if (in_.first.size() <= 0)
	{
		Error("トークン木がないよ！");
		return;
	}
	Read(&in_.first[2], 0);
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth)
{
	assert(n != nullptr
		&& "どこかでnullった！");

	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		printfDx("プロセス\n");
		Read(n->proc.proc, d);
		Read(n->proc.next, d);
		break;// 処理
	case NODE_EXPR:
		printfDx("式\n");
		Read(n->expr.ls, d);
		Read(n->expr.rs, d);
		break;// 式
	case NODE_OR:
		Read(n->expr.rs, d);
		printfDx("or\n");
		Read(n->expr.ls, d);
		break;// ||
	case NODE_AND:
		Read(n->expr.rs, d);
		printfDx("and\n");
		Read(n->expr.ls, d);
		break;// &&
	case NODE_EQUAL:
		Read(n->expr.ls, d);
		printfDx("==\n");
		Read(n->expr.rs, d);
		break;// ==
	case NODE_NOTEQUAL:
		Read(n->expr.ls, d);
		printfDx("!=\n");
		Read(n->expr.rs, d);
		break;// !=
	case NODE_LESSTHAN:
		Read(n->expr.ls, d);
		printfDx("<\n");
		Read(n->expr.rs, d);
		break;// <
	case NODE_GREATERTHEN:
		Read(n->expr.ls, d);
		printfDx(">\n");
		Read(n->expr.rs, d);
		break;// >
	case NODE_LESSEQUAL:
		Read(n->expr.ls, d);
		printfDx("<=\n");
		Read(n->expr.rs, d);
		break;// <=
	case NODE_GREATEREQUAL:
		Read(n->expr.ls, d);
		printfDx(">=\n");
		Read(n->expr.rs, d);
		break;// >=
	case NODE_ADD:
		Read(n->expr.ls, d);
		printfDx("+\n");
		Read(n->expr.rs, d);
		break;// +
	case NODE_SUB:
		Read(n->expr.ls, d);
		printfDx("-\n");
		Read(n->expr.rs, d);
		break;// -
	case NODE_MUL:
		Read(n->expr.ls, d);
		printfDx("*\n");
		Read(n->expr.rs, d);
		break;// *
	case NODE_DIV:
		Read(n->expr.ls, d);
		printfDx("/\n");
		Read(n->expr.rs, d);
		break;// /
	case NODE_NFOR:
		printfDx("for\n");
		Read(n->nfor.init, d);
		Read(n->nfor.expr, d);
		Read(n->nfor.updt, d);
		Read(n->nfor.proc, d);
		break;// for文
	case NODE_NIF:
		printfDx("if\n");
		Read(n->nif.expr, d);
		Read(n->nif.proc, d);
		break;// if文
	case NODE_VARDEC:
		printfDx("変数宣言\n");
		Read(n->varDec.type, d);
		Read(n->varDec.assigns, d);
		break;// 変数宣言
	case NODE_FUNCDEC:
		printfDx("関数定義\n");
		Read(n->fancDec.type, d);
		Read(n->fancDec.name, d);
		Read(n->fancDec.param, d);
		break;// 関数宣言
	//case NODE_VALUE:
		//printfDx("値:%s\n", in_.second[n->tokenIndex_].second.c_str());
		
		//break;// 値
	case NODE_INTEGER:
		printfDx("%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 整数値
	//case NODE_LITER_DIGIT:
	//	printfDx("\n");
	//	break;// 整数リテラル
	case NODE_CALLFUNC:
		printfDx("関数呼び出し:\n");
		Read(n->callFunc.name, d);
		Read(n->callFunc.args, d);
		break;// 関数呼び出し
	case NODE_INCREMENT:
		printfDx("インクリメント:\n");
		Read(n->expr.ls, d);
		break;// ++
	case NODE_DECREMENT:
		printfDx("デクリメント:\n");
		Read(n->expr.ls, d); 
		break;// --
	case NODE_NAME:
		printfDx("名前:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 名
	case NODE_TYPE:
		printfDx("型:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 型
	case NODE_RET:
		printfDx("返却:\n");
		Read(n->ret.expr, d);
		break;// return
	case NODE_ARG:
		printfDx("実引数:\n");
		Read(n->arg.expr, d);
		if (n->arg.next != nullptr)
		{
			printfDx(",\n");
			Read(n->arg.next, d);
		}

		break;// 実引数
	case NODE_ASSIGN:
		printfDx("代入:\n");
		Read(n->assigns.name, d);
		printfDx("=:\n");
		Read(n->assigns.expr, d);
		if (n->assigns.next != nullptr)
		{
			printfDx(",\n");
			Read(n->assigns.next, d);
		}
		break;// 代入式
	case NODE_PARAM:
		printfDx("仮引数\n");
		Read(n->param.varDec, d);
		if (n->param.next != nullptr)
		{
			printfDx(",\n");
			Read(n->param.next, d);
		}
		break;// 仮引数
	case NODE_REGISTER_FUNC_NAME:
		printfDx("登録済み関数名: %s\n", in_.second[n->tokenIndex_].second.c_str());
		break;
	case NODE_REGISTER_VAR_NAME:
		printfDx("登録済み変数名: %s\n", in_.second[n->tokenIndex_].second.c_str());
		break;
	default:
		break;
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
}

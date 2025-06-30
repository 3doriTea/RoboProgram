#include "SemanticAnalyzer.h"
#include <DxLib.h>

void SemanticAnalyzer::Analyze()
{
	if (in_.size() <= 0)
	{
		Error("トークン木がないよ！");
		return;
	}
	Read(&in_[0], 0);
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth)
{
	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		printfDx("プロセス\n");
		Read(n->proc.proc, d);
		Read(n->proc.proc, d);
		break;// 処理
	case NODE_EXPR:
		printfDx("\n");
		break;// 式
	case NODE_OR:
		printfDx("\n");
		break;// ||
	case NODE_AND:
		printfDx("\n");
		break;// &&
	case NODE_EQUAL:
		printfDx("\n");
		break;// ==
	case NODE_NOTEQUAL:
		printfDx("\n");
		break;// !=
	case NODE_LESSTHAN:
		printfDx("\n");
		break;// <
	case NODE_GREATERTHEN:
		printfDx("\n");
		break;// >
	case NODE_LESSEQUAL:
		printfDx("\n");
		break;// <=
	case NODE_GREATEREQUAL:
		printfDx("\n");
		break;// >=
	case NODE_ADD:
		printfDx("\n");
		break;// +
	case NODE_SUB:
		printfDx("\n");
		break;// -
	case NODE_MUL:
		printfDx("\n");
		break;// *
	case NODE_DIV:
		printfDx("\n");
		break;// /
	case NODE_NFOR:
		printfDx("\n");
		break;// for文
	case NODE_NIF:
		printfDx("\n");
		break;// if文
	case NODE_VARDEC:
		printfDx("\n");
		break;// 変数宣言
	case NODE_FUNCDEC:
		printfDx("\n");
		break;// 関数宣言
	case NODE_VALUE:
		printfDx("\n");
		break;// 値
	case NODE_INTEGER:
		printfDx("\n");
		break;// 整数値
	case NODE_LITER_DIGIT:
		printfDx("\n");
		break;// 整数リテラル
	case NODE_CALLFUNC:
		printfDx("\n");
		break;// 関数呼び出し
	case NODE_INCREMENT:
		printfDx("\n");
		break;// ++
	case NODE_DECREMENT:
		printfDx("\n");
		break;// --
	case NODE_NAME:
		printfDx("\n");
		break;// 名
	case NODE_TYPE:
		printfDx("\n");
		break;// 型
	case NODE_RET:
		printfDx("\n");
		break;// return
	case NODE_ARG:
		printfDx("\n");
		break;// 実引数
	case NODE_ASSIGN:
		printfDx("\n");
		break;// 代入式
	case NODE_PARAM:
		printfDx("\n");
		break;// 仮引数
	case NODE_REGISTER_FUNC_NAME:
		printfDx("\n");
		break;
	case NODE_REGISTER_VAR_NAME:
		printfDx("\n");
		break;
	default:
		break;
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
}

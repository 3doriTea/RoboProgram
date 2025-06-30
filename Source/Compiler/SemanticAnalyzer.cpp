#include "SemanticAnalyzer.h"
#include <cassert>
#include <DxLib.h>

void SemanticAnalyzer::Analyze()
{
	if (in_.first.size() <= 0)
	{
		Error("�g�[�N���؂��Ȃ���I");
		return;
	}
	Read(&in_.first[2], 0);
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth)
{
	assert(n != nullptr
		&& "�ǂ�����null�����I");

	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		printfDx("�v���Z�X\n");
		Read(n->proc.proc, d);
		Read(n->proc.next, d);
		break;// ����
	case NODE_EXPR:
		printfDx("��\n");
		Read(n->expr.ls, d);
		Read(n->expr.rs, d);
		break;// ��
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
		break;// for��
	case NODE_NIF:
		printfDx("if\n");
		Read(n->nif.expr, d);
		Read(n->nif.proc, d);
		break;// if��
	case NODE_VARDEC:
		printfDx("�ϐ��錾\n");
		Read(n->varDec.type, d);
		Read(n->varDec.assigns, d);
		break;// �ϐ��錾
	case NODE_FUNCDEC:
		printfDx("�֐���`\n");
		Read(n->fancDec.type, d);
		Read(n->fancDec.name, d);
		Read(n->fancDec.param, d);
		break;// �֐��錾
	//case NODE_VALUE:
		//printfDx("�l:%s\n", in_.second[n->tokenIndex_].second.c_str());
		
		//break;// �l
	case NODE_INTEGER:
		printfDx("%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// �����l
	//case NODE_LITER_DIGIT:
	//	printfDx("\n");
	//	break;// �������e����
	case NODE_CALLFUNC:
		printfDx("�֐��Ăяo��:\n");
		Read(n->callFunc.name, d);
		Read(n->callFunc.args, d);
		break;// �֐��Ăяo��
	case NODE_INCREMENT:
		printfDx("�C���N�������g:\n");
		Read(n->expr.ls, d);
		break;// ++
	case NODE_DECREMENT:
		printfDx("�f�N�������g:\n");
		Read(n->expr.ls, d); 
		break;// --
	case NODE_NAME:
		printfDx("���O:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// ��
	case NODE_TYPE:
		printfDx("�^:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// �^
	case NODE_RET:
		printfDx("�ԋp:\n");
		Read(n->ret.expr, d);
		break;// return
	case NODE_ARG:
		printfDx("������:\n");
		Read(n->arg.expr, d);
		if (n->arg.next != nullptr)
		{
			printfDx(",\n");
			Read(n->arg.next, d);
		}

		break;// ������
	case NODE_ASSIGN:
		printfDx("���:\n");
		Read(n->assigns.name, d);
		printfDx("=:\n");
		Read(n->assigns.expr, d);
		if (n->assigns.next != nullptr)
		{
			printfDx(",\n");
			Read(n->assigns.next, d);
		}
		break;// �����
	case NODE_PARAM:
		printfDx("������\n");
		Read(n->param.varDec, d);
		if (n->param.next != nullptr)
		{
			printfDx(",\n");
			Read(n->param.next, d);
		}
		break;// ������
	case NODE_REGISTER_FUNC_NAME:
		printfDx("�o�^�ς݊֐���: %s\n", in_.second[n->tokenIndex_].second.c_str());
		break;
	case NODE_REGISTER_VAR_NAME:
		printfDx("�o�^�ςݕϐ���: %s\n", in_.second[n->tokenIndex_].second.c_str());
		break;
	default:
		break;
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
}

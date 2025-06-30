#include "SemanticAnalyzer.h"
#include <DxLib.h>

void SemanticAnalyzer::Analyze()
{
	if (in_.size() <= 0)
	{
		Error("�g�[�N���؂��Ȃ���I");
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
		printfDx("�v���Z�X\n");
		Read(n->proc.proc, d);
		Read(n->proc.proc, d);
		break;// ����
	case NODE_EXPR:
		printfDx("\n");
		break;// ��
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
		break;// for��
	case NODE_NIF:
		printfDx("\n");
		break;// if��
	case NODE_VARDEC:
		printfDx("\n");
		break;// �ϐ��錾
	case NODE_FUNCDEC:
		printfDx("\n");
		break;// �֐��錾
	case NODE_VALUE:
		printfDx("\n");
		break;// �l
	case NODE_INTEGER:
		printfDx("\n");
		break;// �����l
	case NODE_LITER_DIGIT:
		printfDx("\n");
		break;// �������e����
	case NODE_CALLFUNC:
		printfDx("\n");
		break;// �֐��Ăяo��
	case NODE_INCREMENT:
		printfDx("\n");
		break;// ++
	case NODE_DECREMENT:
		printfDx("\n");
		break;// --
	case NODE_NAME:
		printfDx("\n");
		break;// ��
	case NODE_TYPE:
		printfDx("\n");
		break;// �^
	case NODE_RET:
		printfDx("\n");
		break;// return
	case NODE_ARG:
		printfDx("\n");
		break;// ������
	case NODE_ASSIGN:
		printfDx("\n");
		break;// �����
	case NODE_PARAM:
		printfDx("\n");
		break;// ������
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

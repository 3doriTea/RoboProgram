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
		Error("�g�[�N���؂��Ȃ���I");
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

	Error("�O���[�o�����Ȃ���I");
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth)
{
	assert(n != nullptr
		&& "�ǂ�����null�����I");

	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		PRINTF("�v���Z�X\n");
		Read(n->proc.proc, d);
		if (n->proc.next != nullptr)  // ���̃v���Z�X������Ȃ�
		{
			Read(n->proc.next, d);
		}
		break;// ����
	case NODE_EXPR:
		PRINTF("��\n");
		Read(n->expr.ls, d);
		Read(n->expr.rs, d);
		break;// ��
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
		break;// for��
	case NODE_NIF:
		PRINTF("if\n");
		Read(n->nif.expr, d);
		Read(n->nif.proc, d);
		break;// if��
	case NODE_VARDEC:
		PRINTF("�ϐ��錾\n");
		Read(n->varDec.type, d);
		Read(n->varDec.assigns, d);
		break;// �ϐ��錾
	case NODE_FUNCDEC:
		PRINTF("�֐���`\n");
		Read(n->fancDec.type, d);
		Read(n->fancDec.name, d);
		if (n->fancDec.param != nullptr)  // �p�����[�^������Ȃ�
		{
			Read(n->fancDec.param, d);
		}
		Read(n->fancDec.proc, d);
		break;// �֐��錾
	//case NODE_VALUE:
		//PRINTF("�l:%s\n", in_.second[n->tokenIndex_].second.c_str());
		
		//break;// �l
	case NODE_INTEGER:
		PRINTF("%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// �����l
	//case NODE_LITER_DIGIT:
	//	PRINTF("\n");
	//	break;// �������e����
	case NODE_CALLFUNC:
		PRINTF("�֐��Ăяo��:\n");
		Read(n->callFunc.name, d);
		if (n->callFunc.args != nullptr)  // ����������Ȃ�
		{
			Read(n->callFunc.args, d);
		}
		break;// �֐��Ăяo��
	case NODE_INCREMENT:
		PRINTF("�C���N�������g:\n");
		Read(n->expr.ls, d);
		break;// ++
	case NODE_DECREMENT:
		PRINTF("�f�N�������g:\n");
		Read(n->expr.ls, d); 
		break;// --
	case NODE_NAME:
		PRINTF("���O:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// ��
	case NODE_TYPE:
		PRINTF("�^:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// �^
	case NODE_RET:
		PRINTF("�ԋp:\n");
		Read(n->ret.expr, d);
		break;// return
	case NODE_ARG:
		PRINTF("������:\n");
		Read(n->arg.expr, d);
		if (n->arg.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->arg.next, d);
		}

		break;// ������
	case NODE_ASSIGN:
		PRINTF("���:\n");
		Read(n->assigns.name, d);
		PRINTF("=:\n");
		Read(n->assigns.expr, d);
		if (n->assigns.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->assigns.next, d);
		}
		break;// �����
	case NODE_PARAM:
		PRINTF("������\n");
		Read(n->param.varDec, d);
		if (n->param.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->param.next, d);
		}
		break;// ������
	case NODE_REGISTER_FUNC_NAME:
		PRINTF("�o�^�ς݊֐���:\n");
		Read(n->func.name, d);
		break;
	case NODE_REGISTER_VAR_NAME:
		PRINTF("�o�^�ςݕϐ���:\n");
		Read(n->var.name, d);
		break;
	case NODE_GLOBAL:
		PRINTF("�O���[�o��\n");
		Read(n->global.funcDef, d);
		if (n->global.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->global.next, d);
		}
		break;
	default:
		assert(false && "����������Ă��Ȃ�");
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
}

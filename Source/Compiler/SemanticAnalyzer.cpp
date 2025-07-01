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

int SemanticAnalyzer::NewMemory(std::string name, int size)
{
	assert(size == 4 && "4byte�ȊO�̌^�T�C�Y�͖������ł��B");

	std::vector<bool> used(maxMemorySize / 4, false);

	int offset = 0;
	for (auto& mem : memory)
	{
		used[mem.second.offset / 4] = true;
		offset++;
	}

	for (int i = 0; i < used.size(); i++)
	{
		// �g���Ă��Ȃ��ꏊ����
		if (used[i] == false)
		{
			memory.insert({ name, UseMemory{ offset * 4, 4 }});
			return i * 4;
		}
	}
	// �����󂫏ꏊ�Ȃ��Ȃ�

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
		


		Read(n->funcDec.type, d);
		Read(n->funcDec.name, d);
		if (n->funcDec.param != nullptr)  // �p�����[�^������Ȃ�
		{
			Read(n->funcDec.param, d);
		}
		Read(n->funcDec.proc, d);
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
	assert(n != nullptr && "nullptr�������I @SemanticAnalyzer::ReadName");
	
	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_���͈͊O������ @SemanticAnalyzer::ReadName");
	
	return in_.second[n->tokenIndex_].second;
}

int SemanticAnalyzer::ReadIntegier(const NODE* n)
{
	assert(n != nullptr && "nullptr�������I @SemanticAnalyzer::ReadName");

	std::string str{ in_.second[n->tokenIndex_].second };

	return std::stoi(str);
}

void SemanticAnalyzer::ReadParam(const NODE* n, ByteCodes& bc)
{
	// TODO: int�^���Ɖ��m�肵�Ă���
	// �X�^�b�N������o���A4byte�����W�X�^�Ɋi�[����
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
	// �I�y�����h(�퉉�Z�q)��������
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
		assert(false && "�v�Z�ł��Ȃ����Z�q");
		break;
	}
}

void SemanticAnalyzer::MemorySet(const int _addr, const int _size, ByteCodes& bc)
{
	// �������� _addr ���烌�W�X�^�� _size �o�C�g������������
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DSET });  // ����
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // �Z�b�g��-������
		bc.push_back({ {}, static_cast<Byte>(i) });  // ���W�X�^�w��
	}
}

void SemanticAnalyzer::MemoryGet(const int _addr, const int _size, ByteCodes& bc)
{
	// �������� _addr ���烌�W�X�^�� _size �o�C�g����ǂݍ���
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DGET });   // ����
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // �Q�b�g��-������
		bc.push_back({ {}, static_cast<Byte>(i) });  // ���W�X�^�w��
	}
}

void SemanticAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, {});
}

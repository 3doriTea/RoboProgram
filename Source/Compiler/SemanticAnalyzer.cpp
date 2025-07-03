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
	out_.clear();

	
	out_.push_back({ {}, BCD_CALL });
	out_.push_back({ {}, 1 });
	out_.push_back({ {}, BCD_HALT });

	if (in_.first.size() <= 0)
	{
		Error("�g�[�N���؂��Ȃ���I");
		return;
	}
	for (auto& nodes : in_.first)
	{
		if (nodes->type_ == NODE_GLOBAL)
		{
			ReadGlobal(nodes, out_.size());
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

void SemanticAnalyzer::RegSet(int value, ByteCodes& bc, int offset)
{
	Byte* val{ reinterpret_cast<Byte*>(&value) };
	for (int i = 0; i < sizeof(int); i++)
	{
		bc.push_back({ {}, BCD_RSET });
		bc.push_back({ {}, static_cast<Byte>(offset + i) });
		bc.push_back({ {}, val[i] });
	}
}

void SemanticAnalyzer::RegSet(const Byte value, ByteCodes& bc, int offset)
{
	bc.push_back({ {}, BCD_RSET });
	bc.push_back({ {}, static_cast<Byte>(offset) });
	bc.push_back({ {}, value });
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth, int position)
{
	assert(n != nullptr
		&& "�ǂ����� null �����I");

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
		//ReadFuncDec(n, position);
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

void SemanticAnalyzer::ReadFuncDec(const NODE* n, ByteCodes& bc, int position)
{
	//std::string funcName = ReadFuncName(n->funcDec.name, bc);
	std::string funcName = ReadName(n->funcDec.name);
	funcGroup.insert({ funcName, {} });

	FuncData& data{ funcGroup[funcName] };
	ByteCodes& funcBlock{ data.byteCodes };
	data.index = position;
	bc.offset = data.index;

	if (n->funcDec.param != nullptr)  // �p�����[�^������Ȃ�
	{
		ReadParam(n->funcDec.param, bc);
	}

	ReadProcs(n->funcDec.proc, funcBlock, data.index);

	funcBlock.push_back({ {}, BCD_RET });

	for (auto&& byteCode : funcBlock)
	{
		bc.push_back(byteCode);
	}
}

std::string SemanticAnalyzer::ReadName(const NODE* n)
{
	assert(n->type_ == NODE_NAME);

	assert(n != nullptr && "nullptr�������I @SemanticAnalyzer::ReadName");
	
	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_���͈͊O������ @SemanticAnalyzer::ReadName");

	return in_.second[n->tokenIndex_].second;
}

std::string SemanticAnalyzer::ReadFuncName(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_REGISTER_FUNC_NAME);

	return ReadName(n->func.name);
}

int SemanticAnalyzer::ReadType(const NODE* n)
{
	assert(n->type_ == NODE_TYPE);

	assert(n != nullptr && "nullptr�������I @SemanticAnalyzer::ReadType");

	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_���͈͊O������ @SemanticAnalyzer::ReadType");

	std::string typeName{ in_.second[n->tokenIndex_].second };

	if (typeName == "void")
		return 0;
	else if (typeName == "int")
		return 4;
	else
		return -1;
}

int SemanticAnalyzer::ReadInteger(const NODE* n)
{
	assert(n != nullptr && "nullptr�������I @SemanticAnalyzer::ReadName");

	std::string str{ in_.second[n->tokenIndex_].second };

	return std::stoi(str);
}

void SemanticAnalyzer::ReadParam(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_PARAM);

	std::string varName = ReadName(n->param.varDec->varDec.assigns->assigns.name);

	int typeSize{ ReadType(n->param.varDec->varDec.type) };

	if (typeSize == 0)  // �^�̃T�C�Y�� 0 �Ȃ� void �w�肳��Ă���
	{
		Error("�֐��̉������� void�^�͎g�p�ł��܂���B");
		return;
	}

	assert(typeSize == 4 && "���Ή��̌^�T�C�Y:4byte�ȊO�̌^���w�肳�ꂽ");

	// TODO: int�^���Ɖ��m�肵�Ă���
	// �X�^�b�N������o���A4byte�����W�X�^�Ɋi�[����
	bc.push_back({ {}, BCD_POPW });
	bc.push_back({ {}, 4 });

	int addr = NewMemory(varName, 4);
	MemorySet(addr, 4, bc);

	// MEMO: �������Ƃ͋t�ŁA�������͑O���珇�ɏ������Ă���
	if (n->param.next != nullptr)
	{
		ReadParam(n->param.next, bc);
	}
}

void SemanticAnalyzer::ReadProcs(const NODE* n, ByteCodes& bc, int begin)
{
	assert(n->type_ == NODE_PROC);

	// �����͈ȉ��̕���
	switch (n->proc.proc->type_)
	{
	case NODE_NFOR:
		ReadNFor(n->proc.proc, bc, begin);
		break;
	case NODE_NIF:
		ReadNIf(n->proc.proc, bc, begin);
		break;
	case NODE_ASSIGN:
		ReadAssign(n->proc.proc, bc, begin);
		break;
	case NODE_VARDEC:
		ReadVarDec(n->proc.proc, bc, true, true);
		break;
	case NODE_RET:
		ReadRet(n->proc.proc, bc);
		break;
	case NODE_CALLFUNC:
		ReadCallFunc(n->proc.proc, bc);
		break;
	case NODE_PROC:
		assert(false && "PROC");
		ReadProcs(n->proc.proc, bc, begin);
		break;
	case NODE_INCREMENT:
		ReadIncrement(n->proc.proc, bc);
		break;
	case NODE_DECREMENT:
		ReadDecrement(n->proc.proc, bc);
		break;
	default:
		assert(false && "��O�������Ă΂ꂽ");
		break;
	}

	// MEMO: �����I�ɏ������Ă���
	if (n->proc.next != nullptr)  // ���̏���������Ȃ�
	{
		ReadProcs(n->proc.next, bc, begin);
	}
}

void SemanticAnalyzer::ReadGlobal(const NODE* n, int begin)
{
	assert(n->type_ == NODE_GLOBAL);

	ByteCodes globalBlock{};

	ReadFuncDec(n->global.funcDef, globalBlock, begin);

	for (auto&& byteCode : globalBlock)
	{
		out_.push_back(byteCode);
	}

	if (n->global.next != nullptr)
	{
		ReadGlobal(n->global.next, begin);
	}
}

void SemanticAnalyzer::ReadRet(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_RET);

	// �߂�l�̎�����������A������̓X�^�b�N�Ƀv�b�V������Ă��邽�߁A���̂܂ܕԋp����
	ReadExpr(n->ret.expr, bc);
}

void SemanticAnalyzer::ReadArg(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_ARG);

	// MEMO: �����̌���D�悵�ăX�^�b�N�Ƀv�b�V�����Ă������ƂŁA�ǂݎ��Ƃ��͑O����ǂݎ���
	if (n->arg.next != nullptr)
	{
		ReadArg(n->arg.next, bc);
	}

	ReadExpr(n->arg.expr, bc);  // ���������Ă��炤
}

void SemanticAnalyzer::ReadNFor(const NODE* n, ByteCodes& bc, int blockBegin)
{
	assert(n->type_ == NODE_NFOR);

	// ����������
	if (n->nfor.init->type_ == NODE_VARDEC)
	{
		ReadVarDec(n->nfor.init, bc, true, false);
	}
	else
	{
		ReadAssign(n->nfor.init, bc, -1);
	}

	// �����m�F
	ByteCodes forExpr{};
	ReadExpr(n->nfor.expr, forExpr);

	// �����m�F�̂��߃��W�X�^�Ɍ��ʂ��|�b�v
	forExpr.push_back({ {}, BCD_POPW });
	forExpr.push_back({ {}, 0 });
	forExpr.push_back({ {}, 4 });

	// ���������s�����Ȃ�u���b�N�𒴂���
	forExpr.push_back({ {}, BCD_CFJP });
	forExpr.push_back({ {}, 0 });  // �Q�ƃ��W�X�^

	ByteCodes forBlock{};
	// �������m�F����O�Ƀu���b�N�m�F
	ReadProcs(n->nfor.proc, forBlock, blockBegin + bc.size());

	if (n->nfor.updt->type_ == NODE_ASSIGN)
	{
		ReadAssign(n->nfor.updt, forBlock, 4);
	}
	else
	{
		ReadExpr(n->nfor.updt, forBlock);
	}

	int destEndInt{ static_cast<int>(forBlock.size()) + 2 };

	signed char destEnd{ static_cast<signed char>(destEndInt) };

	assert((INT8_MIN <= destEndInt) && (destEndInt <= INT8_MAX)
		&& "�W�����v�悪1byte�Ɏ��܂�Ȃ�����");
	
	forExpr.push_back({ {}, static_cast<Byte>(destEnd) });

	// ���������������Ă���
	for (auto&& byteCode : forExpr)
	{
		bc.push_back(byteCode);
	}

	// �������������I������ if�̃u���b�N����
	for (auto&& byteCode : forBlock)
	{
		bc.push_back(byteCode);
	}

	bc.push_back({ {}, BCD_JMP });

	int destBeginInt{ static_cast<int>(forBlock.size() + forExpr.size()) + 1 };

	signed char destBegin{ -static_cast<signed char>(destBeginInt) };

	assert(INT8_MIN <= destBeginInt && destBeginInt <= INT8_MAX
		&& "�W�����v�悪1byte�Ɏ��܂�Ȃ�����");

	bc.push_back({ {}, static_cast<Byte>(destBegin) });
}

void SemanticAnalyzer::ReadNIf(const NODE* n, ByteCodes& bc, int blockBegin)
{
	assert(n->type_ == NODE_NIF);

	ReadExpr(n->nif.expr, bc);

	ByteCodes ifBlock{};
	ReadProcs(n->nif.proc, ifBlock, blockBegin + bc.size());

	// TODO: if�̂Ƃ���̃W�����v�~�X���Ă����炱��������
	bc.push_back({ {}, BCD_POPW });  // ���̌��ʂ����W�X�^�ɓf���o��
	bc.push_back({ {}, 0 });
	bc.push_back({ {}, 4 });

	bc.push_back({ {}, BCD_CFJP });  // ���W�X�^0�Ԃ�false�̂Ƃ��W�����v
	bc.push_back({ {}, 0 });  // 0�ԃ��W�X�^ ���Q��

	assert(ifBlock.size() <= INT8_MAX);

	bc.push_back({ {}, static_cast<Byte>(ifBlock.size()) });  // �u���b�N�v���Z�X����ɃW�����v

	// if�̏������������I������ if�̃u���b�N����
	for (auto&& byteCode : ifBlock)
	{
		bc.push_back(byteCode);
	}
}

void SemanticAnalyzer::ReadExpr(const NODE* n, ByteCodes& bc)
{
	// �I�y�����h(�퉉�Z�q) ��������
	switch (n->type_)
	{
	case NODE_REGISTER_VAR_NAME:
		ReadVar(n, bc);  // �ϐ����Q�Ƃ���
		return;
	case NODE_CALLFUNC:
		ReadCallFunc(n, bc);
		return;
	case NODE_INTEGER:
	{
		int val{ ReadInteger(n) };
		RegSet(val, bc, 0);
		bc.push_back({ {}, BCD_PUSW });
		bc.push_back({ {}, 0 });
		bc.push_back({ {}, 4 });
		return;
	}
	default:
		break;
	}

	// �I�y���[�^(���Z�q) ��������
	switch (n->type_)
	{
	case NODE_ADD:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_ADD });
		break;
	case NODE_SUB:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_SUB });
		break;
	case NODE_MUL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_MUL });
		break;
	case NODE_DIV:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_DIV });
		break;
	case NODE_AND:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_AND });
		break;
	case NODE_OR:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_OR });
		break;
	case NODE_EQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_EQUAL });
		break;
	case NODE_NOTEQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_NOTEQUAL });
		break;
	case NODE_LESSTHAN:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_LESSTHAN });
		break;
	case NODE_GREATERTHEN:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATERTHEN });
		break;
	case NODE_LESSEQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_LESSEQUAL });
		break;
	case NODE_GREATEREQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATEREQUAL });
		break;
	case NODE_INCREMENT:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATEREQUAL });
		break;
	default:
		assert(false && "�v�Z�ł��Ȃ����Z�q");
		break;
	}
}

void SemanticAnalyzer::ReadAssign(const NODE* n, ByteCodes& bc, const int typeSize, const bool newDec)
{
	// TODO: �^�T�C�Y4byte�ȊO�ɂȂ�ꍇ�A�A�������錾���Ɍ^�T�C�Y�������œn���K�v������
	assert(n->type_ == NODE_ASSIGN);

	// TODO: �����������������Ȃ��Ȃ� name �̑O�ɉ�������ł��邩��
	std::string varName = ReadName(n->assigns.name);
	int addr{ -1 };

	if (newDec)
	{
		addr = NewMemory(varName, typeSize);

		assert(addr >= 0 && "�������̊m�ۂ��ł��Ȃ������B");
	}
	else
	{
		addr = GetMemory(varName);

		if (addr < 0)
		{
			Error(std::string{ "�錾����Ă��Ȃ��ϐ��u" } + varName + "�v���Q�Ƃ���܂����B");
			return;
		}
	}


	// �����������Ȃ珈������
	if (n->assigns.expr != nullptr)
	{
		// ���������A������̓X�^�b�N�ɒl���c���Ă���
		ReadExpr(n->assigns.expr, bc);

		// TODO: int�^���Ɖ��m�肵�Ă���
		// �X�^�b�N������o���A4byte�����W�X�^�Ɋi�[����
		bc.push_back({ {}, BCD_POPW });
		bc.push_back({ {}, 0 });
		bc.push_back({ {}, static_cast<Byte>(typeSize) });
		MemorySet(addr, typeSize, bc);
	}

	// �A���錾������Ȃ珈������
	if (n->assigns.next)
	{
		ReadAssign(n->assigns.next, bc, typeSize, newDec);
	}
}

void SemanticAnalyzer::ReadIncrement(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_INCREMENT);

	ReadExpr(n->increment.expr, bc);

	RegSet(1, bc);
	bc.push_back({ {}, BCD_PUSW });
	bc.push_back({ {}, 0 });
	bc.push_back({ {}, 4 });

	bc.push_back({ {}, BCD_ADD });
}

void SemanticAnalyzer::ReadDecrement(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_DECREMENT);

	ReadExpr(n->increment.expr, bc);

	RegSet(1, bc);
	bc.push_back({ {}, BCD_PUSW });
	bc.push_back({ {}, 0 });
	bc.push_back({ {}, 4 });

	bc.push_back({ {}, BCD_SUB });
}

void SemanticAnalyzer::ReadVar(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_REGISTER_VAR_NAME);

	std::string varName{ ReadName(n->var.name) };

	if (varName == "true")
	{
		RegSet(1, bc, 0);
		bc.push_back({ {}, BCD_PUSW });
		bc.push_back({ {}, 0 });
		bc.push_back({ {}, 4 });
		return;
	}
	else if (varName == "false")
	{
		RegSet(0, bc, 0);
		bc.push_back({ {}, BCD_PUSW });
		bc.push_back({ {}, 0 });
		bc.push_back({ {}, 4 });
		return;
	}

	int addr{ GetMemory(varName) };

	if (addr < 0)
	{
		Error(std::string{ "�錾����Ă��Ȃ��ϐ��u" } + varName + "�v���Q�Ƃ���܂����B");
		return;
	}

	// TODO: �^�T�C�Y��4���Ɖ��m�肵�Ă���

	//bc.push_back({ {}, BCD_DGET });
	MemoryGet(addr, 4, bc);  // ���������烌�W�X�^�ɏ����ʂ�

	// ���W�X�^����X�^�b�N�Ƀv�b�V������
	bc.push_back({ {}, BCD_PUSW });
	bc.push_back({ {}, 0 });
	bc.push_back({ {}, 4 });
}

void SemanticAnalyzer::ReadVarDec(const NODE* n, ByteCodes& bc, bool allowInit, bool consecutive)
{
	// TODO: �^�T�C�Y4byte�ȊO�ɂȂ�ꍇ�A�A�������錾���Ɍ^�T�C�Y�������œn���K�v������
	assert(n->type_ == NODE_VARDEC);


	if (allowInit == false)
	{
		// ������������Ă��Ȃ��̂Ɏ��������Ă���
		if (n->varDec.assigns->assigns.expr != nullptr)
		{
			Error(std::string{ "�ϐ��u" } + ReadName(n->varDec.assigns->assigns.name) + "�v�͏������ł��܂���B");
			return;
		}
	}

	if (consecutive == false)
	{
		// �A�������ϐ��錾��������Ă��Ȃ��̂ɁA�A�����Ă���
		if (n->varDec.assigns->assigns.next != nullptr)
		{
			Error("�A���ŕϐ��錾�͂ł��܂���B");
			return;
		}
	}


	int typeSize{ 0 };
	if (n->varDec.type != nullptr)
	{
		typeSize = ReadType(n->varDec.type);
	}

	if (typeSize == 0)  // �^�̃T�C�Y�� 0 �Ȃ� void �w�肳��Ă���
	{
		Error("�ϐ��錾�Ƃ��� void�^�͎g�p�ł��܂���B");
		return;
	}

	assert(typeSize == 4 && "���Ή��̌^�T�C�Y:4byte�ȊO�̌^���w�肳�ꂽ");

	ReadAssign(n->varDec.assigns, bc, typeSize, true);  // �ϐ��A����Ƃ�������
}

void SemanticAnalyzer::ReadCallFunc(const NODE* n, ByteCodes& bc)
{
	std::string funcName = ReadFuncName(n->callFunc.name, bc);

#pragma region ���{��p�R�[�h

	if (funcName == "Run")
	{
		bc.push_back({ {}, BCD_ACT });
		bc.push_back({ {}, BCD_ACT_RUN });
		return;
	}
	else if (funcName == "Jump")
	{
		bc.push_back({ {}, BCD_ACT });
		bc.push_back({ {}, BCD_ACT_JUMP });
		return;
	}
	else if (funcName == "Skip")
	{
		bc.push_back({ {}, BCD_NOP });
		return;
	}
	else if (funcName == "Stop")
	{
		bc.push_back({ {}, BCD_HALT });
		return;
	}
	else if (funcName == "IsOnGround")
	{
		bc.push_back({ {}, BCD_AIO });
		bc.push_back({ {}, BCD_AIO_ISGROUND });
		return;
	}
	else if (funcName == "CheckTile")
	{
		bc.push_back({ {}, BCD_AIO });
		bc.push_back({ {}, BCD_AIO_CHECKTILE });
		return;
	}

#pragma endregion

	if (funcGroup.count(funcName) != 1)
	{
		Error(std::string("�֐�:") + funcName + "�͖���`�ł��B");
		return;
	}

	if (funcGroup[funcName].index < 0)
	{
		Error(std::string("�֐�:") + funcName + "�͖������ł��B");
		return;
	}

	// NOTE: �֐��Ăяo���̈ʒu����Ă����炱��������
	int current{ static_cast<int>(bc.offset + bc.size()) };
	int diff{ funcGroup[funcName].index - current };  // �Ăяo���֐��̑��Έʒu�����߂�

	assert(INT8_MIN <= diff && diff <= INT8_MAX
		&& "�֐��̈ʒu��8bit�ŕ\����͈͊O�ɂȂ��Ă��܂���");

	// �֐��̈ʒu�͌��܂���

	// ���������Z�b�g����
	ReadArg(n->callFunc.args, bc);

	bc.push_back({ {}, BCD_CALL });
	bc.push_back({ {}, static_cast<Byte>(diff) });
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

//void SemanticAnalyzer::StackPush(const int _regOffset, const int _size, ByteCodes& bc)
//{
//	for (int i = 0; i < _size; i++)
//	{
//		bc.push_back({ {}, BCD_PUSW });  // ����
//		bc.push_back({ {}, _regOffset + i });
//		bc.push_back({ {}, _size });
//	}
//}

void SemanticAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, {});
}

void SemanticAnalyzer::Error(const std::string& _message)
{
	Error(_message.c_str());
}

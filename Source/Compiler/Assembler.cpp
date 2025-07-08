#include "Assembler.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>
#include "../CodeReader/ByteCodeReader.h"
#include "../ByteCodeDefine.h"


namespace
{
	static const int ABYTE_WIDTH{ 2 };  // 1byte ��\�����߂̕�����
	static const int NAME_WIDTH{ 6 };  // ���O��\��������
	static const int PARAM_HINT_WIDTH{ 3 };  // �p�����[�^�q���g�̕�����
	static const int PARAM_VALUE_WIDTH{ 2 };  // �p�����[�^�����̕�����
	static const int LINE_COUNT_WIDTH{ 3 };  // �s�����̕�����
}

void Assembler::ToString(const ByteCodes& bcs, std::string& outStr)
{
	std::stringstream ss{};

	std::vector<Byte> bytes{};

	for (auto&& bc : bcs)
	{
		bytes.push_back(bc.second);
	}

	// �P�ɏo�͂���
	auto Print = [&](const Byte code)
		{
			ss << std::hex << std::setw(ABYTE_WIDTH) << std::setfill('0')
				<< std::uppercase << static_cast<int>(code) << " ";
		};

	// ���O���o�͂���
	auto PrintN = [&](const std::string& name)
		{
			ss << std::setw(NAME_WIDTH) << std::setfill(' ') << std::uppercase << name << " ";
		};

	// �p�����[�^���q���g��Y���ďo�͂���
	auto PrintC = [&](const std::string& hint, const Byte code)
		{
			ss << std::setw(PARAM_HINT_WIDTH) << std::setfill(' ') << std::uppercase << hint << ":";
			ss << std::hex << std::setw(PARAM_VALUE_WIDTH) << std::setfill('0') << std::uppercase << static_cast<int>(code) << " ";
		};

	ByteCodeReader bcr{ bytes };

	while (!bcr.IsEndOfCode())
	{
		ss << std::setw(LINE_COUNT_WIDTH) << std::setfill(' ') << std::dec << bcr.GetCurrentIndex();//bc.first.line;
		ss << ":";
		Byte code{ bcr.Pop() };
		switch (code)
		{
		case BCD_NOP:
		case BCD_HALT:
		case BCD_RET:
		case BCD_ADD:
		case BCD_SUB:
		case BCD_MUL:
		case BCD_DIV:
		case BCD_AND:
		case BCD_OR:
		case BCD_EQUAL:
		case BCD_NOTEQUAL:
		case BCD_LESSTHAN:
		case BCD_GREATERTHEN:
		case BCD_LESSEQUAL:
		case BCD_GREATEREQUAL:
		{
			Print(code);
		}
		break;
		case BCD_ACT:
		case BCD_AIO:
		case BCD_PUS:
		case BCD_POP:
		case BCD_JMP:
		{
			Print(code);
			Print(bcr.Pop());
		}
		break;
		case BCD_CFJP:
		case BCD_CTJP:
		case BCD_DSET:
		case BCD_DGET:
		case BCD_PUSW:
		case BCD_POPW:
		case BCD_RSET:
		{
			Print(code);
			Print(bcr.Pop());
			Print(bcr.Pop());
		}
		break;
		case BCD_CALL:
		{
			Print(code);
			Print(bcr.Pop());
			Print(bcr.Pop());
			Print(bcr.Pop());
			Print(bcr.Pop());
		}
		break;
		default:
			assert(false && "����`");
			break;
		}
		ss << std::endl;
	}

	bcr.Seek(0);

	ss << std::endl << std::endl;

	while (!bcr.IsEndOfCode())
	{
		ss << std::setw(LINE_COUNT_WIDTH) << std::setfill(' ') << std::dec << bcr.GetCurrentIndex();//bc.first.line;
		ss << ":";
		Byte code{ bcr.Pop() };
		switch (code)
		{
		case BCD_NOP:
			PrintN("NOP");
			break;
		case BCD_HALT:
			PrintN("HALT");
			break;
		case BCD_RET:
			PrintN("RET");
			break;
		case BCD_ADD:
			PrintN("ADD");
			break;
		case BCD_SUB:
			PrintN("SUB");
			break;
		case BCD_MUL:
			PrintN("MUL");
			break;
		case BCD_DIV:
			PrintN("DIV");
			break;
		case BCD_AND:
			PrintN("AND");
			break;
		case BCD_OR:
			PrintN("OR");
			break;
		case BCD_EQUAL:
			PrintN("=");
			break;
		case BCD_NOTEQUAL:
			PrintN("NOT=");
			break;
		case BCD_LESSTHAN:
			PrintN("<");
			break;
		case BCD_GREATERTHEN:
			PrintN(">");
			break;
		case BCD_LESSEQUAL:
			PrintN("<=");
			break;
		case BCD_GREATEREQUAL:
			PrintN(">=");
			break;
// --------------------------------------------
		case BCD_ACT:
			PrintN("ACT");
			PrintC("act", bcr.Pop());
			break;
		case BCD_AIO:
			PrintN("AIO");
			PrintC("io", bcr.Pop());
			break;
		case BCD_PUS:
			PrintN("PUSH");
			PrintC("reg", bcr.Pop());
			break;
		case BCD_POP:
			PrintN("POP");
			PrintC("reg", bcr.Pop());
			break;
		case BCD_JMP:
			PrintN("JMP");
			PrintC("to", bcr.Pop());
			break;
// --------------------------------------------
		case BCD_CFJP:
			PrintN("JMPIFF");
			PrintC("reg", bcr.Pop());
			PrintC("to", bcr.Pop());
			break;
		case BCD_CTJP:
			PrintN("JMPIFT");
			PrintC("reg", bcr.Pop());
			PrintC("to", bcr.Pop());
			break;
		case BCD_DSET:
			PrintN("MEMSET");
			PrintC("mem", bcr.Pop());
			PrintC("reg", bcr.Pop());
			break;
		case BCD_DGET:
			PrintN("MEMGET");
			PrintC("mem", bcr.Pop());
			PrintC("reg", bcr.Pop());
			break;
		case BCD_PUSW:
			PrintN("PUSHW");
			PrintC("reg", bcr.Pop());
			PrintC("siz", bcr.Pop());
			break;
		case BCD_POPW:
			PrintN("POPW");
			PrintC("reg", bcr.Pop());
			PrintC("siz", bcr.Pop());
			break;
		case BCD_RSET:
			PrintN("REGSET");
			PrintC("reg", bcr.Pop());
			PrintC("val", bcr.Pop());
			break;
// --------------------------------------------
		case BCD_CALL:
			PrintN("CALL");
			PrintC("at", bcr.Pop());
			PrintC("", bcr.Pop());
			PrintC("", bcr.Pop());
			PrintC("", bcr.Pop());
			break;
		default:
			assert(false && "����`");
			break;
		}
		ss << std::endl;
	}

	outStr = ss.str();
}

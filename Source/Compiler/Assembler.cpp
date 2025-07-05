#include "Assembler.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>
#include "../CodeReader/ByteCodeReader.h"
#include "../ByteCodeDefine.h"

void Assembler::ToAssemble(const ByteCodes& bcs, std::string& outStr)
{
	std::stringstream ss{};

	std::vector<Byte> bytes{};

	for (auto&& bc : bcs)
	{
		bytes.push_back(bc.second);
	}

	auto Print = [&](Byte code)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(code) << " ";
		};

	auto PrintN = [&](std::string name)
		{
			ss << std::setw(6) << std::setfill(' ') << std::uppercase << name << " ";
		};
	auto PrintC = [&](std::string hint, Byte code)
		{
			ss << std::setw(3) << std::setfill(' ') << std::uppercase << hint << ":";
			ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(code) << " ";
		};

	ByteCodeReader bcr{ bytes };

	while (!bcr.IsEndOfCode())
	{
		ss << std::setw(3) << std::setfill(' ') << std::dec << bcr.GetCurrentIndex();//bc.first.line;
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
			assert(false && "–¢’è‹`");
			break;
		}
		ss << std::endl;
	}

	bcr.Seek(0);

	ss << std::endl << std::endl;

	while (!bcr.IsEndOfCode())
	{
		ss << std::setw(3) << std::setfill(' ') << std::dec << bcr.GetCurrentIndex();//bc.first.line;
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
			assert(false && "–¢’è‹`");
			break;
		}
		ss << std::endl;
	}

	outStr = ss.str();
}

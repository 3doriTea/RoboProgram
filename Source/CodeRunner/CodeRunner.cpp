#include "CodeRunner.h"
#include <cassert>
#include <DxLib.h>


CodeRunner::CodeRunner(
	ByteCodeReader& _byteCodeReader,
	std::vector<Byte>& _memory,
	Stack<int>& _stackMachine,
	Stack<int>& _callStack,
	std::vector<Byte>& _register,
	const int _registerSize) :
	bcr_{ _byteCodeReader },
	memory_{ _memory },
	stackMachine_{ _stackMachine },
	callStack_{ _callStack },
	register_{ _register },
	REGISTER_SIZE{ _registerSize }
{
}

CodeRunner::~CodeRunner()
{
}

bool CodeRunner::TryReadNext()
{
	if (bcr_.IsEndOfCode())
	{
		return false;  // 終了地点なら失敗
	}

	if (bcr_.Consume(BCD_NOP))
	{
		// 何もしない
	}
	else if (bcr_.Consume(BCD_HALT))
	{
		using SeekPoint = ByteCodeReader::SeekPoint;
		// 終了する
		bcr_.SeekTo(SeekPoint::Tail);
	}
	else if (bcr_.Consume(BCD_ACT))
	{
		if (bcr_.Consume(BCD_ACT_NONE))
		{
			// 何もしない
		}
		else if (bcr_.Consume(BCD_ACT_JUMP))
		{
			onActionMessage_(ActionMessage::Jump);
		}
		else if (bcr_.Consume(BCD_ACT_RUN))
		{
			onActionMessage_(ActionMessage::Run);
		}
		else
		{
			// 未定義
			assert(false && "未定義: BCD_ACT_???");
		}
	}
	else if (bcr_.Consume(BCD_AIO))
	{
		if (bcr_.Consume(BCD_AIO_CHECKTILE))
		{
			int result{ onGetIOMessage_(GetIOMessage::CheckTile) };
			stackMachine_.Push(result);
		}
		else if (bcr_.Consume(BCD_AIO_ISGROUND))
		{
			int result{ onGetIOMessage_(GetIOMessage::IsGrounded) };
			stackMachine_.Push(result);
		}
		else if (bcr_.Consume(BCD_AIO_ONTILENUMBER))
		{
			int result{ onGetIOMessage_(GetIOMessage::GetOnTileNumber) };
			stackMachine_.Push(result);
		}
		else
		{
			// 未定義
			assert(false && "未定義: BCD_AIO_???");
		}
	}
	else if (bcr_.Consume(BCD_CALL))
	{
		int dest{};  // 関数スタートの相対位置
		Byte* destPtr{ reinterpret_cast<Byte*>(&dest) };
		destPtr[0] = bcr_.Pop();
		destPtr[1] = bcr_.Pop();
		destPtr[2] = bcr_.Pop();
		destPtr[3] = bcr_.Pop();

		callStack_.Push(bcr_.GetCurrentIndex());
		bcr_.Seek(dest + bcr_.GetCurrentIndex());
	}
	else if (bcr_.Consume(BCD_RET))
	{
		Byte retAddr = callStack_.Pop();
		bcr_.Seek(retAddr);
	}

	else if (bcr_.Consume(BCD_JMP))
	{
		signed char destAddr = static_cast<signed char>(bcr_.Pop());  // ジャンプ先の相対位置

		bcr_.Seek(destAddr + bcr_.GetCurrentIndex());
	}

	else if (bcr_.Consume(BCD_CFJP))
	{
		Byte conRegAddr{ bcr_.Pop() };
		signed char destAddr = static_cast<signed char>(bcr_.Pop());  // ジャンプ先の相対位置

		Byte value{ register_[conRegAddr] };

		if (!value)  // 条件が false のときジャンプ
		{
			bcr_.Seek(destAddr + bcr_.GetCurrentIndex());
		}
	}

	else if (bcr_.Consume(BCD_CTJP))
	{
		Byte conRegAddr{ bcr_.Pop() };
		signed char destAddr = static_cast<signed char>(bcr_.Pop());  // ジャンプ先の相対位置

		Byte value{ register_[conRegAddr] };

		if (value)  // 条件が true のときジャンプ
		{
			bcr_.Seek(destAddr + bcr_.GetCurrentIndex());
		}
	}

	/*else if (bcr_.Consume(BCD_JP))
	{
		// ジャンプ処理
	}*/
	else if (bcr_.Consume(BCD_DSET))
	{
		Byte destMemAddr{ bcr_.Pop() };
		Byte fromRegAddr{ bcr_.Pop() };
		memory_[destMemAddr] = register_[fromRegAddr];
	}
	else if (bcr_.Consume(BCD_DGET))
	{
		Byte fromMemAddr{ bcr_.Pop() };
		Byte destRegAddr{ bcr_.Pop() };
		register_[destRegAddr] = memory_[fromMemAddr];
	}
	else if (bcr_.Consume(BCD_PUS))
	{
		Byte fromRegAddr{ bcr_.Pop() };
		stackMachine_.Push(register_[fromRegAddr]);
	}
	else if (bcr_.Consume(BCD_PUSW))
	{
		Byte fromRegAddr{ bcr_.Pop() };
		Byte fromRegSize{ bcr_.Pop() };
		int tmp{};
		::memcpy(&tmp, &(register_.data()[fromRegAddr]), fromRegSize);
		stackMachine_.Push(tmp);
	}
#pragma region 四則演算
	else if (bcr_.Consume(BCD_ADD))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a + b);
	}
	else if (bcr_.Consume(BCD_SUB))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a - b);
	}
	else if (bcr_.Consume(BCD_MUL))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a * b);
	}
	else if (bcr_.Consume(BCD_DIV))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a / b);
	}
#pragma endregion
#pragma region 論理演算
	else if (bcr_.Consume(BCD_AND))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a && b);
	}
	else if (bcr_.Consume(BCD_OR))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a || b);
	}
	else if (bcr_.Consume(BCD_EQUAL))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a == b);
	}
	else if (bcr_.Consume(BCD_NOTEQUAL))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a != b);
	}
	else if (bcr_.Consume(BCD_LESSTHAN))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a < b);
	}
	else if (bcr_.Consume(BCD_GREATERTHEN))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a > b);
	}
	else if (bcr_.Consume(BCD_LESSEQUAL))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a <= b);
	}
	else if (bcr_.Consume(BCD_GREATEREQUAL))
	{
		int b{ stackMachine_.Pop() };
		int a{ stackMachine_.Pop() };
		stackMachine_.Push(a >= b);
	}
#pragma endregion
	else if (bcr_.Consume(BCD_POP))
	{
		Byte destRegAddr{ bcr_.Pop() };
		register_[destRegAddr] = static_cast<Byte>(stackMachine_.Pop());
	}
	else if (bcr_.Consume(BCD_POPW))
	{
		Byte destRegAddr{ bcr_.Pop() };
		Byte destRegSize{ bcr_.Pop() };
		int tmp{ stackMachine_.Pop() };
		::memcpy(&(register_.data()[destRegAddr]), &tmp, destRegSize);
	}
	else if (bcr_.Consume(BCD_RSET))
	{
		Byte destRegAddr{ bcr_.Pop() };
		Byte destValue{ bcr_.Pop() };
		register_[destRegAddr] = destValue;
	}
	else
	{
		//assert(false && "未定義のバイトコード");
		printfDx("未定義のバイトコード%d:0x%02x\n", bcr_.GetCurrentIndex(), bcr_.SafePeek());
	}

	return true;
}

void CodeRunner::Reset()
{
	bcr_.Seek(0);
	memory_.clear();
	memory_.resize(256);  // 256 byteメモリ
	stackMachine_.Clear();
	callStack_.Clear();
	register_.clear();
	register_.resize(REGISTER_SIZE, 0);
}

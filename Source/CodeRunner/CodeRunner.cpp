#include "CodeRunner.h"
#include <cassert>


CodeRunner::CodeRunner(
	const std::vector<Byte>& _byteCode,
	std::vector<Byte>& _memory,
	Stack<int>& _stackMachine,
	Stack<Byte>& _callStack,
	std::vector<Byte>& _register,
	const int _registerSize) :
	bcr_{ _byteCode },
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
		return false;  // �I���n�_�Ȃ玸�s
	}

	if (bcr_.Consume(BCD_NOP))
	{
		// �������Ȃ�
	}
	else if (bcr_.Consume(BCD_HALT))
	{
		// ����`
		assert(false && "����`: BCD_HALT");
	}
	else if (bcr_.Consume(BCD_ACT))
	{
		if (bcr_.Consume(BCD_ACT_NONE))
		{
			// �������Ȃ�
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
			// ����`
			assert(false && "����`: BCD_ACT_???");
		}
	}
	else if (bcr_.Consume(BCD_CALL))
	{
		signed char destAddr = static_cast<signed char>(bcr_.Pop());  // �֐��X�^�[�g�̑��Έʒu
		callStack_.Push(bcr_.GetCurrentIndex());
		bcr_.Seek(destAddr + bcr_.GetCurrentIndex());
	}
	else if (bcr_.Consume(BCD_RET))
	{
		Byte retAddr = callStack_.Pop();
		bcr_.Seek(retAddr);
	}
	/*else if (bcr_.Consume(BCD_JP))
	{
		// �W�����v����
	}*/
	else if (bcr_.Consume(BCD_DSET))
	{
		Byte destMemAddr = bcr_.Pop();
		Byte fromRegAddr = bcr_.Pop();
		memory_[destMemAddr] = register_[fromRegAddr];
	}
	else if (bcr_.Consume(BCD_DGET))
	{
		Byte fromMemAddr = bcr_.Pop();
		Byte destRegAddr = bcr_.Pop();
		register_[destRegAddr] = memory_[fromMemAddr];
	}
	else if (bcr_.Consume(BCD_PUS))
	{
		Byte fromRegAddr = bcr_.Pop();
		stackMachine_.Push(register_[fromRegAddr]);
	}
	else if (bcr_.Consume(BCD_PUSW))
	{
		Byte fromRegAddr = bcr_.Pop();
		Byte fromRegSize = bcr_.Pop();
		int tmp{};
		::memcpy(&tmp, &(register_.data()[fromRegAddr]), fromRegSize);
		stackMachine_.Push(tmp);
	}

	return true;
}

void CodeRunner::Reset()
{
	bcr_.Seek(0);
	memory_.clear();
	stackMachine_.Clear();
	callStack_.Clear();
	register_.clear();
	register_.resize(REGISTER_SIZE, 0);
}

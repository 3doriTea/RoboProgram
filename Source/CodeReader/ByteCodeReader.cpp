#include "ByteCodeReader.h"
#include <cassert>

ByteCodeReader::ByteCodeReader(
	const std::vector<Byte>& _byteCode,
	const size_t _current) :
	byteCode_{ _byteCode },
	current_{ _current }
{
}

ByteCodeReader::~ByteCodeReader()
{
}

bool ByteCodeReader::IsEndOfCode() const
{
	return current_ >= byteCode_.size();
}

bool ByteCodeReader::Consume(const Byte _code)
{
	if (IsEndOfCode())
	{
		return false;
	}

	if (byteCode_[current_] == _code)
	{
		current_++;
		return true;
	}
	return false;
}

Byte ByteCodeReader::Pop()
{
	Byte byte = byteCode_[current_];
	current_++;

	return byte;
}

Byte ByteCodeReader::SafePeek() const
{
	if (IsEndOfCode())
	{
		return 0xFF;
	}
	return byteCode_[current_];
}

void ByteCodeReader::Seek(const size_t _index)
{
	// TODO: �͈͓����`�F�b�N����
	current_ = _index;
}

void ByteCodeReader::SeekTo(const SeekPoint _seekPoint)
{
	switch (_seekPoint)
	{
	case SeekPoint::Head:
		Seek(0);
		break;
	case SeekPoint::Tail:
		Seek(byteCode_.size());  // IsEndOfCode��true�ɂȂ邽�ߖ����ȃC���f�b�N�X��OK
		break;
	default:
		assert(false && "�����Ȏw��ʒu");
		break;
	}
}

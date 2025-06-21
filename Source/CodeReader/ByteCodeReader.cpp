#include "ByteCodeReader.h"

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

void ByteCodeReader::Seek(const size_t _index)
{
	// TODO: 範囲内かチェックする
	current_ = _index;
}

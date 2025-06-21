#include "SourceObserver.h"
#include <fstream>
#include <cassert>
#include "../Utility/VectorUtil.h"
#include <DxLib.h>


namespace
{
	static const size_t BUFFER_SIZE{ 1024 };  // 1�s�̕�����o�b�t�@�̃T�C�Ybyte
}

SourceObserver::SourceObserver(const std::string& _fileName) :
	fileName_{ _fileName }
{
}

SourceObserver::~SourceObserver()
{
}

void SourceObserver::Update()
{
	using VectorUtil::CopyVector;

	int hFile = FileRead_open(fileName_.c_str());
	assert(hFile != 0
		&& "�t�@�C�����J���̂Ɏ��s���� @SourceObserver::Update");

	FileRead_set_format(hFile, DX_CHARCODEFORMAT_UTF8);  // UTF-8 �ŊJ��

	std::vector<std::string> lines{};

	char buffer[BUFFER_SIZE]{};  // ������̃o�b�t�@
	while (!FileRead_eof(hFile))
	{
		int length = FileRead_gets(buffer, BUFFER_SIZE, hFile);
		assert(length != -1
			&& "�t�@�C������1�s�擾�Ɏ��s @SourceObserver::Update");

		lines.push_back(buffer);
		ZeroMemory(buffer, BUFFER_SIZE);  // �ǉ��������ƃ[������
	}

	FileRead_close(hFile);  // �t�@�C�������

	if (lines.size() != prevSourceLines_.size())
	{
		CopyVector(prevSourceLines_, lines);
		onUpdateSource_(prevSourceLines_);
		return;
	}

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines[i] != prevSourceLines_[i])
		{
			CopyVector(prevSourceLines_, lines);
			onUpdateSource_(prevSourceLines_);
			return;
		}
	}
}

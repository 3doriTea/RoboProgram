#include "FileSaver.h"
#include <fstream>

FileSaver::FileSaver(const std::string& _fileName, const size_t _bufferSize) :
	FILE_NAME_{ _fileName },
	BUFFER_SIZE_{ _bufferSize }
{
}

FileSaver::~FileSaver()
{
}

bool FileSaver::TryLoad()
{
	using mtbin::Byte;

	std::ifstream ifs(FILE_NAME_, std::ios::binary);
	if (!ifs)
	{
		return false;  // 開けなかった / ファイルがなかった
	}

	// ファイルサイズ計測
	ifs.seekg(0, std::ios::end);
	size_t size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	if (size != BUFFER_SIZE_)
	{
		ifs.close();  // ファイルサイズが違う
		return false;
	}

	Byte* pBuffer = new Byte[BUFFER_SIZE_];
	ifs.read(reinterpret_cast<char*>(pBuffer), BUFFER_SIZE_);
	ifs.close();

	mtbin::MemoryStream ms{ pBuffer, BUFFER_SIZE_ };
	onLoad_(ms);

	delete[] pBuffer;

	return true;
}

bool FileSaver::TrySave()
{
	using mtbin::Byte;

	std::ofstream ofs(FILE_NAME_, std::ios::binary);
	if (!ofs)
	{
		return false;  // 開けなかった
	}

	Byte* pBuffer = new Byte[BUFFER_SIZE_];

	mtbin::MemoryStream ms{ pBuffer, BUFFER_SIZE_ };
	onSave_(ms);

	ofs.write(reinterpret_cast<char*>(pBuffer), BUFFER_SIZE_);
	ofs.close();

	delete[] pBuffer;

	return true;
}

void FileSaver::QuickWriteText(
	const std::string& _fileName,
	const std::string& _text)
{
	std::ofstream ofs(_fileName);
	if (ofs)  // 開けているなら
	{
		ofs << _text;
		ofs.close();
	}
}

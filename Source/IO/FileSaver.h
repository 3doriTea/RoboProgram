#pragma once
#include <string>
#include <functional>
#include "MemoryStream.h"

using FileStreamCallback = std::function<void(mtbin::MemoryStream&)>;

class FileSaver
{
public:
	FileSaver(const std::string& _fileName, const size_t _bufferSize);
	~FileSaver();

	bool TryLoad();
	bool TrySave();

	/// <summary>
	/// ���[�h�����Ƃ��̏���
	/// </summary>
	/// <param name="_callback">void(mtbin::MemoryStream&)</param>
	inline void OnLoad(FileStreamCallback _callback) { onLoad_ = _callback; }
	/// <summary>
	/// �Z�[�u�����Ƃ��̏���
	/// </summary>
	/// <param name="_callback">void(mtbin::MemoryStream&)</param>
	inline void OnSave(FileStreamCallback _callback) { onSave_ = _callback; }

	static void QuickWriteText(const std::string& _fileName, const std::string& _text);
private:
	FileStreamCallback onLoad_;  // ���[�h����
	FileStreamCallback onSave_;  // �Z�[�u����

	const std::string FILE_NAME_;  // ���[�h�A�Z�[�u�Ώۂ̃t�@�C���p�X
	const size_t BUFFER_SIZE_;  // �Z�[�u�t�@�C���̃o�b�t�@�T�C�Y
};

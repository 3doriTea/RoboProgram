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
	/// ロードしたときの処理
	/// </summary>
	/// <param name="_callback">void(mtbin::MemoryStream&)</param>
	inline void OnLoad(FileStreamCallback _callback) { onLoad_ = _callback; }
	/// <summary>
	/// セーブしたときの処理
	/// </summary>
	/// <param name="_callback">void(mtbin::MemoryStream&)</param>
	inline void OnSave(FileStreamCallback _callback) { onSave_ = _callback; }

	static void QuickWriteText(const std::string& _fileName, const std::string& _text);
private:
	FileStreamCallback onLoad_;  // ロード処理
	FileStreamCallback onSave_;  // セーブ処理

	const std::string FILE_NAME_;  // ロード、セーブ対象のファイルパス
	const size_t BUFFER_SIZE_;  // セーブファイルのバッファサイズ
};

#pragma once
#include <vector>


using Byte = unsigned char;

/// <summary>
/// バイトコードを読み取るためのストリームクラス
/// </summary>
class ByteCodeReader
{
public:
	ByteCodeReader(const std::vector<Byte>& _byteCode, const size_t _current = 0);
	~ByteCodeReader();

	/// <summary>
	/// コードの終端に到達しているか
	/// </summary>
	/// <returns>到達している true / false</returns>
	bool IsEndOfCode() const;
	/// <summary>
	/// 引数と一致したら進める、一致しなかったら進めない
	/// </summary>
	/// <param name="_code">比較コード</param>
	/// <returns>一致した true / false</returns>
	bool Consume(const Byte _code);

	/// <summary>
	/// 現在のインデックスを取得
	/// </summary>
	/// <returns>現在見ているコードのインデックス</returns>
	size_t GetCurrentIndex() const { return current_; }

	void Seek(const size_t _index);

private:
	const std::vector<Byte>& byteCode_;  // 読み取るバイトコード
	size_t current_;  // 現在の位置
};

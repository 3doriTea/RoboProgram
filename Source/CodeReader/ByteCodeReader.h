#pragma once
#include <vector>


using Byte = unsigned char;

/// <summary>
/// バイトコードを読み取るためのストリームクラス
/// </summary>
class ByteCodeReader
{
public:
	enum struct SeekPoint
	{
		Head,
		Tail,
	};

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
	/// 単純に出力して進める
	/// </summary>
	/// <returns>現在見ている</returns>
	Byte Pop();

	/// <summary>
	/// 現在のインデックスを取得
	/// </summary>
	/// <returns>現在見ているコードのインデックス</returns>
	size_t GetCurrentIndex() const { return current_; }

	/// <summary>
	/// 現在見ているインデックスを指定する
	/// </summary>
	/// <param name="_index"></param>
	void Seek(const size_t _index);
	/// <summary>
	/// 現在見ているインデックスを相対的に指定する
	/// </summary>
	/// <param name="_seekPoint"></param>
	void SeekTo(const SeekPoint _seekPoint);

private:
	const std::vector<Byte>& byteCode_;  // 読み取るバイトコード
	size_t current_;  // 現在の位置
};

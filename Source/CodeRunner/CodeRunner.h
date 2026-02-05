#pragma once
#include <functional>
#include <vector>
#include <string>
#include "../ByteCodeDefine.h"
#include "../CodeReader/ByteCodeReader.h"
#include "Stack.h"


/// <summary>
/// アクションメッセージ
/// </summary>
enum struct ActionMessage
{
	Run,
	Jump,
};

/// <summary>
/// 入出力メッセージ
/// </summary>
enum struct GetIOMessage
{
	IsGrounded,
	CheckTile,
	GetOnTileNumber,
};

/// <summary>
/// VM本体
/// </summary>
class CodeRunner
{
private:
	enum ErrorExitCode
	{
		ERR_ON_RUNNING = -3104,  // 実行中エラー
	};

public:
	CodeRunner(
		ByteCodeReader& _byteCodeReader,  // コード領域
		std::vector<Byte>& _memory,       // メモリ領域
		Stack<int>& _stackMachine,        // スタック領域0
		Stack<int>& _callStack,           // スタック領域1
		std::vector<Byte>& _register,     // レジスタ
		const int _registerSize,          // レジスタのサイズ
		const int _memorySize             // メモリのサイズ
	);
	~CodeRunner();

	/// <summary>
	/// 試しに次のバイトコードを読み進める
	/// </summary>
	/// <returns>読み進めた true / false</returns>
	bool TryReadNext();
	/// <summary>
	/// 読み取り状態をリセットする
	/// </summary>
	void Reset();

	/// <summary>
	/// 今から見るバイトコードを、読み進めず取得する
	/// </summary>
	/// <returns>バイトコード</returns>
	inline Byte PeekCurrent() const { return bcr_.SafePeek(); }

	/// <summary>
	/// アクションメッセージを受け取ったとき
	/// </summary>
	/// <param name="_callback">void(const ActionMessage)</param>
	/// <returns>自身の参照ポインタ</returns>
	inline CodeRunner& OnActionMessage(const std::function<void(const ActionMessage)>& _callback) { onActionMessage_ = _callback; return *this; }
	/// <summary>
	/// 入出力メッセージを受け取ったとき
	/// </summary>
	/// <param name="_callback">int(const GetIOMessage)</param>
	/// <returns>自身の参照ポインタ</returns>
	inline CodeRunner& OnGetIOMessage(const std::function<int(const GetIOMessage)>& _callback) { onGetIOMessage_ = _callback; return *this; }
	/// <summary>
	/// 現在読み取っているインデクスを取得する
	/// </summary>
	/// <returns>読み取っているインデクス</returns>
	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }

	/// <summary>
	/// エラーにより読み取りを終了
	/// </summary>
	/// <param name="_errorCode">エラーコード</param>
	/// <param name="_message">エラー内容</param>
	void ExitByError(const int _errorCode, const std::string& _message);

private:
	const int REGISTER_SIZE;  // レジスタのサイズ
	const int MEMORY_SIZE;    // メモリ領域のサイズ

	std::function<void(const ActionMessage)> onActionMessage_;  // ロボアクションのコールバック関数
	std::function<int(const GetIOMessage)> onGetIOMessage_;     // 入出力メッセージのコールバック関数

	ByteCodeReader& bcr_;          // コード領域リーダー
	std::vector<Byte>& memory_;    // メモリ領域
	Stack<int>& stackMachine_;     // スタック領域0
	Stack<int>& callStack_;        // スタック領域1
	std::vector<Byte>& register_;  // レジスタ
};

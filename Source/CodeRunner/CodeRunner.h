#pragma once
#include "../ByteCodeDefine.h"
#include "../CodeReader/ByteCodeReader.h"
#include <functional>
#include <vector>
#include <string>
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
		std::vector<Byte>& _memory,          // メモリ領域
		Stack<int>& _stackMachine,           // スタック領域0
		Stack<int>& _callStack,             // スタック領域1
		std::vector<Byte>& _register,        // レジスタ
		const int _registerSize  // レジスタのサイズ
	);
	~CodeRunner();

	bool TryReadNext();
	void Reset();

	inline Byte PeekCurrent() const { return bcr_.SafePeek(); }

	/// <summary>
	/// アクションメッセージを受け取ったとき
	/// </summary>
	/// <param name="_callback">void(const ActionMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnActionMessage(const std::function<void(const ActionMessage)>& _callback) { onActionMessage_ = _callback; return *this; }
	/// <summary>
	/// 入出力メッセージを受け取ったとき
	/// </summary>
	/// <param name="_callback">int(const GetIOMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnGetIOMessage(const std::function<int(const GetIOMessage)>& _callback) { onGetIOMessage_ = _callback; return *this; }
	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }

	void ExitByError(const int _errorCode, const std::string& _message);
private:
	const int REGISTER_SIZE;  // レジスタのサイズ

	std::function<void(const ActionMessage)> onActionMessage_;
	std::function<int(const GetIOMessage)> onGetIOMessage_;

	ByteCodeReader& bcr_;          // コード領域リーダー
	std::vector<Byte>& memory_;    // メモリ領域
	Stack<int>& stackMachine_;     // スタック領域0
	Stack<int>& callStack_;        // スタック領域1
	std::vector<Byte>& register_;  // レジスタ
};

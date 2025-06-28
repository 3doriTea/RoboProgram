#pragma once
#include "../ByteCodeDefine.h"
#include "../CodeReader/ByteCodeReader.h"
#include <functional>
#include <vector>
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
/// VM本体
/// </summary>
class CodeRunner
{
public:
	CodeRunner(
		ByteCodeReader& _byteCodeReader,  // コード領域
		std::vector<Byte>& _memory,          // メモリ領域
		Stack<int>& _stackMachine,           // スタック領域0
		Stack<Byte>& _callStack,             // スタック領域1
		std::vector<Byte>& _register,        // レジスタ
		const int _registerSize  // レジスタのサイズ
	);
	~CodeRunner();

	bool TryReadNext();
	void Reset();

	/// <summary>
	/// アクションメッセージを受け取ったとき
	/// </summary>
	/// <param name="_callback">void(const ActionMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnActionMessage(const std::function<void(const ActionMessage)>& _callback) { onActionMessage_ = _callback; }
	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }
private:
	const int REGISTER_SIZE;  // レジスタのサイズ

	std::function<void(const ActionMessage)> onActionMessage_;

	ByteCodeReader& bcr_;          // コード領域リーダー
	std::vector<Byte>& memory_;    // メモリ領域
	Stack<int>& stackMachine_;     // スタック領域0
	Stack<Byte>& callStack_;       // スタック領域1
	std::vector<Byte>& register_;  // レジスタ
};

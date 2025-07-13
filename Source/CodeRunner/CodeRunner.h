#pragma once
#include "../ByteCodeDefine.h"
#include "../CodeReader/ByteCodeReader.h"
#include <functional>
#include <vector>
#include <string>
#include "Stack.h"

/// <summary>
/// �A�N�V�������b�Z�[�W
/// </summary>
enum struct ActionMessage
{
	Run,
	Jump,
};

/// <summary>
/// ���o�̓��b�Z�[�W
/// </summary>
enum struct GetIOMessage
{
	IsGrounded,
	CheckTile,
	GetOnTileNumber,
};

/// <summary>
/// VM�{��
/// </summary>
class CodeRunner
{
private:
	enum ErrorExitCode
	{
		ERR_ON_RUNNING = -3104,  // ���s���G���[
	};

public:
	CodeRunner(
		ByteCodeReader& _byteCodeReader,  // �R�[�h�̈�
		std::vector<Byte>& _memory,          // �������̈�
		Stack<int>& _stackMachine,           // �X�^�b�N�̈�0
		Stack<int>& _callStack,             // �X�^�b�N�̈�1
		std::vector<Byte>& _register,        // ���W�X�^
		const int _registerSize  // ���W�X�^�̃T�C�Y
	);
	~CodeRunner();

	bool TryReadNext();
	void Reset();

	inline Byte PeekCurrent() const { return bcr_.SafePeek(); }

	/// <summary>
	/// �A�N�V�������b�Z�[�W���󂯎�����Ƃ�
	/// </summary>
	/// <param name="_callback">void(const ActionMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnActionMessage(const std::function<void(const ActionMessage)>& _callback) { onActionMessage_ = _callback; return *this; }
	/// <summary>
	/// ���o�̓��b�Z�[�W���󂯎�����Ƃ�
	/// </summary>
	/// <param name="_callback">int(const GetIOMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnGetIOMessage(const std::function<int(const GetIOMessage)>& _callback) { onGetIOMessage_ = _callback; return *this; }
	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }

	void ExitByError(const int _errorCode, const std::string& _message);
private:
	const int REGISTER_SIZE;  // ���W�X�^�̃T�C�Y

	std::function<void(const ActionMessage)> onActionMessage_;
	std::function<int(const GetIOMessage)> onGetIOMessage_;

	ByteCodeReader& bcr_;          // �R�[�h�̈惊�[�_�[
	std::vector<Byte>& memory_;    // �������̈�
	Stack<int>& stackMachine_;     // �X�^�b�N�̈�0
	Stack<int>& callStack_;        // �X�^�b�N�̈�1
	std::vector<Byte>& register_;  // ���W�X�^
};

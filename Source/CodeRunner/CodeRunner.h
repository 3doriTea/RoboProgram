#pragma once
#include "../ByteCodeDefine.h"
#include "../CodeReader/ByteCodeReader.h"
#include <functional>
#include <vector>
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
/// VM�{��
/// </summary>
class CodeRunner
{
public:
	CodeRunner(
		ByteCodeReader& _byteCodeReader,  // �R�[�h�̈�
		std::vector<Byte>& _memory,          // �������̈�
		Stack<int>& _stackMachine,           // �X�^�b�N�̈�0
		Stack<Byte>& _callStack,             // �X�^�b�N�̈�1
		std::vector<Byte>& _register,        // ���W�X�^
		const int _registerSize  // ���W�X�^�̃T�C�Y
	);
	~CodeRunner();

	bool TryReadNext();
	void Reset();

	/// <summary>
	/// �A�N�V�������b�Z�[�W���󂯎�����Ƃ�
	/// </summary>
	/// <param name="_callback">void(const ActionMessage)</param>
	/// <returns></returns>
	inline CodeRunner& OnActionMessage(const std::function<void(const ActionMessage)>& _callback) { onActionMessage_ = _callback; }
	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }
private:
	const int REGISTER_SIZE;  // ���W�X�^�̃T�C�Y

	std::function<void(const ActionMessage)> onActionMessage_;

	ByteCodeReader& bcr_;          // �R�[�h�̈惊�[�_�[
	std::vector<Byte>& memory_;    // �������̈�
	Stack<int>& stackMachine_;     // �X�^�b�N�̈�0
	Stack<Byte>& callStack_;       // �X�^�b�N�̈�1
	std::vector<Byte>& register_;  // ���W�X�^
};

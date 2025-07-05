#pragma once
#include "Rectan.h"
#include <vector>
#include <functional>
#include "CodeReader/ByteCodeReader.h"
#include "CodeRunner/CodeRunner.h"


using Byte = unsigned char;

class Stage;

class Robot
{
public:
	Robot(
		Rectan& _rect,
		float& _moveSpeed,
		float& _jumpV0,
		Stage*& _pStage,
		bool& _isGrounded,
		Vector2& _velocity,
		bool& _prevPushedSpace,
		const std::vector<Byte>& _byteCode);
	~Robot();

	void Update();

	bool TryReadNext();
	void Reset();

	void Run();
	void Jump();
	void ChangeDir();
	void SetDir(const bool _isLeft);

	inline void GetMemoryRef(const std::function<void(
			const ByteCodeReader& _codeReader,
			const std::vector<Byte>& _memory,
			const Stack<int>& _stackMachine,
			const Stack<int>& _callStack,
			const std::vector<Byte>& _register)>& _callback) const;

	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }

private:
	static const int REGISTER_SIZE;  // ���W�X�^�̃T�C�Y

	bool isLeftDir_;  // �����������Ă���
	bool toJump_;  // �W�����v����
	float runningTimeLeft_;
	Rectan& rect_;
	float& moveSpeed_;  // �ړ��X�s�[�h
	float& jumpV0_;  // �W�����v����
	Stage*& pStage_;
	bool& isGrounded_;
	Vector2& velocity_;
	bool& prevPushedSpace_;

	ByteCodeReader bcr_;
	std::vector<Byte> memory_;
	Stack<int> stackMachine_;
	Stack<int> callStack_;
	std::vector<Byte> register_;

	CodeRunner codeRunner_;  // �R�[�h�����s���铪
};

#pragma once
#include "Rectan.h"
#include <vector>
#include "CodeReader/ByteCodeReader.h"


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

	inline int GetReadByteCodeIndex() const { return static_cast<int>(bcr_.GetCurrentIndex()); }

private:
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
};

#pragma once
#include "Object2D.h"
#include <vector>
#include "Robot.h"
#include "Utility/Timer.h"
#include "Compiler/Analyzer.h"
#include "ViewerBox.h"


struct SOURCE_POS;

class Player : public Object2D
{
private:
	enum State : int
	{
		S_READY,
		S_RUN,
		S_ERROR,
		S_DOWN,  // ���Ă���
		S_MAX,
	};

public:
	Player(const Vector2& _position);
	~Player();

	void Update() override;
	void Draw() override;

	/// <summary>
	/// �v���C���[�̐g�����擾
	/// </summary>
	/// <returns>�g��(pixel)</returns>
	int GetHeight() const;
	/// <summary>
	/// �v���C���[�̉������擾
	/// </summary>
	/// <returns>����(pixel)</returns>
	int GetWidth() const;

	void SetError(const std::string& _message, const SOURCE_POS& _srcPos);
	void SetByteCode(const ByteCodes& _byteCode);

	int GetReadLine() const;

	void ShockDown();

	/// <summary>
	/// �r���[�A���Z�b�g����
	/// </summary>
	/// <param name="_pSrcCodeViewer"></param>
	/// <param name="_pByteCodeViewer"></param>
	/// <param name="_pMemoryViewer"></param>
	/// <param name="_pCallStackViewer"></param>
	/// <param name="_pStackMachineViewer"></param>
	/// <param name="_pRegisterViewer"></param>
	void SetViewerBoxes(
		ViewerBox* _pSrcCodeViewer,
		ViewerBox* _pByteCodeViewer,
		ViewerBox* _pMemoryViewer,
		ViewerBox* _pCallStackViewer,
		ViewerBox* _pStackMachineViewer,
		ViewerBox* _pRegisterViewer);

private:
	State GetState() const { return currentState_; }
	void SetState(const State _state);

private:
	ViewerBox* pSrcCodeViewer_;
	ViewerBox* pByteCodeViewer_;
	ViewerBox* pMemoryViewer_;
	ViewerBox* pCallStackViewer_;
	ViewerBox* pStackMachineViewer_;
	ViewerBox* pRegisterViewer_;

	bool isShockDown_;

	TimerHandle hBeatTimer_;

	int sleepCount_;
	bool prevIsSucceedTryRead;

	State currentState_;
	int hStateImages[S_MAX];

	Vector2 velocity_;
	bool isGrounded;
	bool prevPushedSpace;

	//Vector2 position_;
	Robot robot_;
	
	float gravity;
	float jumpHeight;
	float moveSpeed;
	float jumpV0;

	ByteCodes byteCodeAndLines_;
	std::vector<Byte> byteCode_;

	bool hasByteCodeViewer_{ true };
	bool hasMemoryViewer_{ true };
	bool hasCallStackViewer_{};
	bool hasRegisterViewer_{};
	bool hasStackMachineViewer_{};
};

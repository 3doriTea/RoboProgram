#pragma once
#include "Object2D.h"
#include <vector>
#include "Robot.h"
#include "Utility/Timer.h"

struct SOURCE_POS;

class Player : public Object2D
{
private:
	enum State : int
	{
		S_READY,
		S_RUN,
		S_ERROR,
		S_MAX,
	};

public:
	Player(const Vector2& _position);
	~Player();

	void Update() override;
	void Draw() override;

	/// <summary>
	/// プレイヤーの身長を取得
	/// </summary>
	/// <returns>身長(pixel)</returns>
	int GetHeight() const;
	/// <summary>
	/// プレイヤーの横幅を取得
	/// </summary>
	/// <returns>横幅(pixel)</returns>
	int GetWidth() const;

	void SetError(const std::string& _message, const SOURCE_POS& _srcPos);
	void SetByteCode(const std::vector<std::pair<int, Byte>>& _byteCode);

	int GetReadLine() const;

private:
	State GetState() const { return currentState_; }
	void SetState(const State _state);

private:
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

	std::vector<std::pair<int, Byte>> byteCodeAndLines_;
	std::vector<Byte> byteCode_;
};

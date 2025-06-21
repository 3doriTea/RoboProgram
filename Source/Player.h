#pragma once
#include "Object2D.h"
#include <vector>
#include "Robot.h"


class Player : public Object2D
{
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

	void SetByteCode(const std::vector<std::pair<int, Byte>>& _byteCode);

	int GetReadLine() const;

private:

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

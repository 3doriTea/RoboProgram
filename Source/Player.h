#pragma once
#include "Object2D.h"


class Player : public Object2D
{
public:
	Player(const Vector2& _position);
	~Player();

	void Update() override;
	void Draw() override;

private:
	Vector2 velocity_;
	bool isGrounded;
	bool prevPushedSpace;

	//Vector2 position_;
	
	float gravity;
	float jumpHeight;
	float moveSpeed;
	float jumpV0;
};

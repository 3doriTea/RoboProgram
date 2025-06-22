#pragma once
#include "Object2D.h"


class Coin : public Object2D
{
public:
	Coin(const Vector2& _position);
	~Coin();

	void Update() override;
};

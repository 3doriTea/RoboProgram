#pragma once
#include "Object2D.h"

class BlockEffect : public Object2D
{
public:
	BlockEffect(const Vector2Int& _tilePosition, unsigned int _color);
	~BlockEffect();

	void Update() override;
	void Draw() override;

private:
	float timeLeft_;  // エフェクトが消えるまでの時間
	unsigned int color_;  // エフェクトの色
};

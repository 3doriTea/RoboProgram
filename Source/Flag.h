#pragma once
#include "StageObject.h"

class Flag : StageObject
{
public:
	Flag(const Vector2& _position);
	~Flag();

	void Draw() override;

	void OnPlayer() override;
};

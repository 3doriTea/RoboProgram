#pragma once
#include "StageObject.h"


class Goal : public StageObject
{
public:
	Goal(const Vector2& _position);
	~Goal();

	void Draw() override;
	void OnPlayer() override;
};

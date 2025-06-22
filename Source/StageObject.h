#pragma once
#include "Object2D.h"
#include "Player.h"
#include "Stage.h"
#include <cassert>


class StageObject : public Object2D
{
public:
	StageObject(const Vector2& _position, const char* _imageFileName);
	~StageObject();

	void Update() override;

	Player* GetPlayer();

	virtual void OnPlayer();
};

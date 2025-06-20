#pragma once
#include "../Library/GameObject.h"
#include "Vector2.h"
#include "Rectan.h"


class Stage;

class Object2D : public GameObject
{
public:
	Object2D();
	virtual ~Object2D();

	virtual void Update() override;
	virtual void Draw() override;

	Vector2 GetPosition() const { return rect_.pivot; }

protected:
	int hImage_;
	Rectan rect_;

protected:
	Stage* pStage_;
};

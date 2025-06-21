#pragma once
#include "../Library/GameObject.h"
#include "Vector2.h"
#include "Vector2Int.h"
#include "Rectan.h"
#include "RectInt.h"


using Byte = unsigned char;

class Stage;

class Object2D : public GameObject
{
public:
	Object2D();
	virtual ~Object2D();

	virtual void Update() override;
	virtual void Draw() override;

	Vector2 GetPosition() const { return rect_.pivot; }
	Rectan GetRect() const { return rect_; }

	Vector2 ToWorld(const Vector2& _localPosition) const;

protected:
	int hImage_;
	Rectan rect_;

protected:
	Stage* pStage_;
};

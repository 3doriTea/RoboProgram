#pragma once
#include "Object2D.h"


class Doc : public Object2D
{
public:
	Doc(const Vector2& _position);
	~Doc();

	void Update() override;
};

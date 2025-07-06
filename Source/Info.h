#pragma once
#include "StageObject.h"


class Info : public StageObject
{
public:
	Info(const Vector2& _position, const int _infoLevel);
	~Info();

	void Draw() override;
	void OnPlayer() override;

private:
	const int INFO_LEVEL_;
};

#pragma once
#include "StageObject.h"


class Lightning : public StageObject
{
public:
	Lightning(const Vector2& _position);
	~Lightning();

	void Update() override;
	void Draw() override;
	void OnPlayer() override;

private:
	bool isShocking_;
	float toCurrSceneTimeLeft_;
};

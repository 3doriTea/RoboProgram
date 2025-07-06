#pragma once
#include "StageObject.h"


class Doc : public StageObject
{
public:
	Doc(const Vector2& _position, const int _docLevel);
	~Doc();

	void Draw() override;
	void OnPlayer() override;

private:
	const int DOC_LEVEL_;
};

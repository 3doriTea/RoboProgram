#pragma once
#include "Object2D.h"

class MessageBoxUI : public Object2D
{
public:
	MessageBoxUI();
	~MessageBoxUI();

	void Update() override;
	void Draw() override;
};

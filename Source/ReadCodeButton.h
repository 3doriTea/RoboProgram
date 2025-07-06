#pragma once
#include "Button.h"

class ReadCodeButton : public Button
{
public:
	ReadCodeButton();
	~ReadCodeButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

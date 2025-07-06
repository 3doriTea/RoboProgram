#pragma once
#include "Button.h"

class FinalTestButton : public Button
{
public:
	FinalTestButton();
	~FinalTestButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

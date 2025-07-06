#pragma once
#include "Button.h"

class ReloadButton : public Button
{
public:
	ReloadButton();
	~ReloadButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

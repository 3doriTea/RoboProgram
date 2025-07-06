#pragma once
#include "Button.h"

class WriteButton : public Button
{
public:
	WriteButton();
	~WriteButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

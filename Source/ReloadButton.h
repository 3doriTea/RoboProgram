#pragma once
#include "Button.h"


/// <summary>
/// UI‚â‚è’¼‚µƒ{ƒ^ƒ“
/// </summary>
class ReloadButton : public Button
{
public:
	ReloadButton();
	~ReloadButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

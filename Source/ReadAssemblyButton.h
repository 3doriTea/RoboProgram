#pragma once
#include "Button.h"

class ReadAssemblyButton : public Button
{
public:
	ReadAssemblyButton();
	~ReadAssemblyButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

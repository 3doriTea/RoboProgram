#pragma once
#include "Button.h"


/// <summary>
/// UIアセンブリを見るボタン
/// </summary>
class ReadAssemblyButton : public Button
{
public:
	ReadAssemblyButton();
	~ReadAssemblyButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;
};

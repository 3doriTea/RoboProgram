#pragma once
#include "Button.h"

class DocButton : public Button
{
public:
	DocButton();
	~DocButton();

	void OnDraw(const bool _onTouching, const bool _isPushing) override;
	void OnPush() override;

	void News() { hImage_ = hImageNew_; }
private:
	int hImageNormal_;
	int hImageNew_;
};

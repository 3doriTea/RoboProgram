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
	/// <summary>
	/// �j���[�X�����邩�`�F�b�N����
	/// </summary>
	/// <returns></returns>
	const bool CheckHasNews() const;
	int hImageNormal_;
	int hImageNew_;
};

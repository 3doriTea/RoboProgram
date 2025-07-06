#pragma once
#include "Object2D.h"

class Button : public Object2D
{
public:
	Button();
	virtual ~Button();

	void Update() override final;
	void Draw() override final;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="_onTouching">�{�^�����G��Ă��� true / false</param>
	/// <param name="_onTouching">�{�^����������Ă��� true / false</param>
	virtual void OnDraw(const bool _onTouching, const bool _isPushing) {}
	/// <summary>
	/// �����ꂽ�Ƃ��̏���
	/// </summary>
	virtual void OnPush() {}

	inline void SetHintText(const std::string _text) { hintText_ = _text; }

private:
	std::string hintText_;  // �}�E�X�I�[�o�[�����Ƃ��̃q���g�e�L�X�g
	Vector2 mousePos_;  // �}�E�X���W
	bool isOnMouse_;  // �}�E�X���G��Ă��邩
	bool isPushing_;  // ������Ă��邩
};

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
	/// 描画処理
	/// </summary>
	/// <param name="_onTouching">ボタンが触れている true / false</param>
	/// <param name="_onTouching">ボタンが押されている true / false</param>
	virtual void OnDraw(const bool _onTouching, const bool _isPushing) {}
	/// <summary>
	/// 押されたときの処理
	/// </summary>
	virtual void OnPush() {}

	inline void SetHintText(const std::string _text) { hintText_ = _text; }

private:
	std::string hintText_;  // マウスオーバーしたときのヒントテキスト
	Vector2 mousePos_;  // マウス座標
	bool isOnMouse_;  // マウスが触れているか
	bool isPushing_;  // 押されているか
};

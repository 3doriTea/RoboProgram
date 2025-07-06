#pragma once
#include "../Library/GameObject.h"
#include "RectInt.h"
#include "Vector2.h"
#include <string>

class CursorText : public GameObject
{
public:
	CursorText();
	~CursorText();

	inline void SetText(const std::string& _text) { text_ = _text; }

	void Update() override;
	void Draw() override;
private:
	std::string text_;
	Vector2 mousePos_;
	RectInt textBox_;
};

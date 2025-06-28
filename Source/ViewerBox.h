#pragma once
#include "Object2D.h"
#include <string>
#include <map>
#include <vector>

using ColorCode = unsigned int;
using BackgroundColor = ColorCode;
using TextColor = ColorCode;

class ViewerBox : public Object2D
{
	enum struct Pivot
	{
		Center,
		BottomCenter,
		RightCenter,
		TopLeft,  // ����
	};
public:
	ViewerBox();
	~ViewerBox();

	void Update() override;
	void Draw() override;

	/// <summary>
	/// ���݌��Ă���s�����w�肷��
	/// </summary>
	/// <param name="_line">-1�ŉ������Ă��Ȃ�</param>
	void ReadLine(const int _line = -1);

	/// <summary>
	/// �{�b�N�X�̈ʒu�����߂�
	/// </summary>
	/// <param name="_position">�ʒu</param>
	/// <param name="pivotType">�ʒu�w�肷��{�b�N�X�̊</param>
	void SetPosition(const Vector2Int _position, const Pivot pivotType);

	ViewerBox& SetFrameWidth(const int _width)                         { frameWidth_ = _width; return *this; }
	ViewerBox& SetFrameColor(const BackgroundColor _color)             { frameColor_ = _color; return *this; }
	ViewerBox& SetDefaultBackgroundColor(const BackgroundColor _color) { defaultBackgroundColor_ = _color; return *this; }
	ViewerBox& SetDefaultTextColor(const TextColor _color)             { defaultTextColor_ = _color; return *this; }
	ViewerBox& SetTextLines(const std::vector<std::string> _textLines) { textLines_ = _textLines; return *this; }

	ViewerBox& ClearMarks();

	ViewerBox& AddMarkLine(
		const int _line,
		const BackgroundColor _backgroundColor,
		const TextColor _textColor);
	// TODO: ViewerBox& AddMarkChar(const int _line, const int _column, const ColorCode _color);

	RectInt GetDrawRect() const { return drawRect_; }

private:
	int readingLine_;  // ���Ă���s��

	int frameWidth_;  // �g�̑���
	BackgroundColor frameColor_;
	BackgroundColor defaultBackgroundColor_;  // �f�t�H���g�̔w�i�F
	TextColor defaultTextColor_;              // �f�t�H���g�̃e�L�X�g�F

	RectInt drawRect_;
	std::vector<std::string> textLines_;
	std::map<int, std::pair<BackgroundColor, ColorCode>> lineMarks_;
};

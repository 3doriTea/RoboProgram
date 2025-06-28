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
		TopLeft,  // 左上
	};
public:
	ViewerBox();
	~ViewerBox();

	void Update() override;
	void Draw() override;

	/// <summary>
	/// 現在見ている行数を指定する
	/// </summary>
	/// <param name="_line">-1で何も見ていない</param>
	void ReadLine(const int _line = -1);

	/// <summary>
	/// ボックスの位置を決める
	/// </summary>
	/// <param name="_position">位置</param>
	/// <param name="pivotType">位置指定するボックスの基準</param>
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
	int readingLine_;  // 見ている行数

	int frameWidth_;  // 枠の太さ
	BackgroundColor frameColor_;
	BackgroundColor defaultBackgroundColor_;  // デフォルトの背景色
	TextColor defaultTextColor_;              // デフォルトのテキスト色

	RectInt drawRect_;
	std::vector<std::string> textLines_;
	std::map<int, std::pair<BackgroundColor, ColorCode>> lineMarks_;
};

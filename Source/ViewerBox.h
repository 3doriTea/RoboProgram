#pragma once
#include "Object2D.h"
#include <string>
#include <map>
#include <vector>


using ColorCode = unsigned int;
using BackgroundColor = ColorCode;
using TextColor = ColorCode;
using Mark = std::pair<BackgroundColor, ColorCode>;

class ViewerBox : public Object2D
{
public:
	enum struct Pivot
	{
		Center,
		BottomCenter,
		RightCenter,
		TopLeft,  // 左上
		BottomRight,  // 右下
		BottomLeft,  // 左下
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
	ViewerBox& ReadLine(const int _line = -1);

	/// <summary>
	/// ボックスの位置を決める
	/// </summary>
	/// <param name="_position">位置</param>
	/// <param name="pivotType">位置指定するボックスの基準</param>
	ViewerBox& SetPosition(const Vector2Int _position, const Pivot _pivotType);

	ViewerBox& SetUseGhost(const bool _use)                            { useGhost_ = _use; return Recalculate(); }
	ViewerBox& SetIsShowLineCountBar(const bool _isShowing)            { isShowLineCountBar_ = _isShowing; return Recalculate(); }
	ViewerBox& SetShowLineCount(const int _width)                      { showLineCount_ = _width; return Recalculate(); }
	ViewerBox& SetTextBoxMargin(const int _margin)                     { textBoxMargin_ = _margin; return Recalculate(); }
	ViewerBox& SetFrameWidth(const int _width)                         { frameWidth_ = _width; return Recalculate(); }
	ViewerBox& SetFrameColor(const BackgroundColor _color)             { frameColor_ = _color; return Recalculate(); }
	ViewerBox& SetDefaultBackgroundColor(const BackgroundColor _color) { defaultBackgroundColor_ = _color; return Recalculate(); }
	ViewerBox& SetDefaultTextColor(const TextColor _color)             { defaultTextColor_ = _color; return Recalculate(); }
	ViewerBox& SetIsShow(const bool _isShowing)                        { isShow_ = _isShowing; return Recalculate(); }
	ViewerBox& SetIsScrollable(const bool _isScrollable)               { isScrollable_ = _isScrollable; return Recalculate(); }
	ViewerBox& SetTextLines(const std::vector<std::string>& _textLines);

	ViewerBox& ClearMarks();

	ViewerBox& AddMarkLine(
		const int _line,
		const BackgroundColor _backgroundColor,
		const TextColor _textColor);
	// TODO: ViewerBox& AddMarkChar(const int _line, const int _column, const ColorCode _color);

	inline RectInt GetDrawRect() const { return rect_.ToInt(); }

	/// <summary>
	/// 配置を再計算する
	/// </summary>
	/// <returns></returns>
	ViewerBox& Recalculate();
private:
	bool useGhost_;  // 透明化するか
	bool isShow_;  // 表示するか
	bool isShowLineCountBar_;  // 行数表示があるか
	bool isScrollable_;  // スクロール可能か

	int showLineCount_;  // 表示する最大の行数 0 以下で全て表示
	int readingLine_;  // 見ている行数
	int lineSize_;  // 行サイズ
	int lineMarginSize_;  // 行間サイズ
	int lineCount_;  // 行数
	int textBoxMargin_;  // テキストボックスの余白

	Vector2Int textBoxSize_;  // テキストだけのサイズ

	int frameWidth_;  // 枠の太さ
	BackgroundColor frameColor_;
	BackgroundColor defaultBackgroundColor_;  // デフォルトの背景色
	TextColor defaultTextColor_;              // デフォルトのテキスト色

	Vector2 position_;  // 指定座標
	Pivot pivotType_;
	std::vector<std::string> textLines_;
	std::map<int, Mark> lineMarks_;

	int drawAlpha_;  // 描画時の不透明度
};

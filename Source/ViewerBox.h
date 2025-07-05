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
	ViewerBox& ReadLine(const int _line = -1);

	/// <summary>
	/// �{�b�N�X�̈ʒu�����߂�
	/// </summary>
	/// <param name="_position">�ʒu</param>
	/// <param name="pivotType">�ʒu�w�肷��{�b�N�X�̊</param>
	ViewerBox& SetPosition(const Vector2Int _position, const Pivot _pivotType);

	ViewerBox& SetIsShowLineCountBar(const bool _isShowing)            { isShowLineCountBar_ = _isShowing; return Recalculate(); }
	ViewerBox& SetShowLineCount(const int _width)                      { showLineCount_ = _width; return Recalculate(); }
	ViewerBox& SetFrameWidth(const int _width)                         { frameWidth_ = _width; return Recalculate(); }
	ViewerBox& SetFrameColor(const BackgroundColor _color)             { frameColor_ = _color; return Recalculate(); }
	ViewerBox& SetDefaultBackgroundColor(const BackgroundColor _color) { defaultBackgroundColor_ = _color; return Recalculate(); }
	ViewerBox& SetDefaultTextColor(const TextColor _color)             { defaultTextColor_ = _color; return Recalculate(); }
	ViewerBox& SetTextLines(const std::vector<std::string>& _textLines);


	ViewerBox& ClearMarks();

	ViewerBox& AddMarkLine(
		const int _line,
		const BackgroundColor _backgroundColor,
		const TextColor _textColor);
	// TODO: ViewerBox& AddMarkChar(const int _line, const int _column, const ColorCode _color);

	inline RectInt GetDrawRect() const { return rect_.ToInt(); }

	/// <summary>
	/// �z�u���Čv�Z����
	/// </summary>
	/// <returns></returns>
	ViewerBox& Recalculate();
private:
	bool isShow_;  // �\�����邩
	bool isShowLineCountBar_;  // �s���\�������邩

	int showLineCount_;  // �\������ő�̍s�� 0 �ȉ��őS�ĕ\��
	int readingLine_;  // ���Ă���s��
	int lineSize_;  // �s�T�C�Y
	int lineMarginSize_;  // �s�ԃT�C�Y
	int lineCount_;  // �s��
	int textBoxMargin_;  // �e�L�X�g�{�b�N�X�̗]��

	Vector2Int textBoxSize_;  // �e�L�X�g�����̃T�C�Y

	int frameWidth_;  // �g�̑���
	BackgroundColor frameColor_;
	BackgroundColor defaultBackgroundColor_;  // �f�t�H���g�̔w�i�F
	TextColor defaultTextColor_;              // �f�t�H���g�̃e�L�X�g�F

	Pivot pivotType_;
	std::vector<std::string> textLines_;
	std::map<int, Mark> lineMarks_;
};

#pragma once
#include "Object2D.h"
#include <vector>
#include <string>


class Player;

class CodeBox : public Object2D
{
public:
	CodeBox();
	~CodeBox();

	void Update() override;
	void Draw() override;

	void SetSourceLines(
		const std::vector<std::string>& _lines);

private:
	enum PositionStyle
	{
		PS_PLAYER,
		PS_TOP,
		PS_BOTTOM,
	};

	/// <summary>
	/// テキストボックスの横幅を取得
	/// </summary>
	/// <returns>pixel</returns>
	int GetTextBoxWidth() const;
	/// <summary>
	/// テキストボックスの高さを取得
	/// </summary>
	/// <returns>pixel</returns>
	int GetTextBoxHeight() const;
	/// <summary>
	/// テキストボックスの描画開始点を取得
	/// </summary>
	/// <returns>描画座標</returns>
	Vector2Int GetDrawTextBegin(const PositionStyle _positionStyle) const;
	/// <summary>
	/// フレームなしの描画矩形
	/// </summary>
	/// <returns></returns>
	RectInt GetDrawRect(const PositionStyle _positionStyle) const;

	std::vector<std::string> sourceLines_;
	int sourceMaxLength_;  // TODO: もし最大幅で失敗したら、最大の行番号を保存する、そこから幅計算
	int sourceMaxLengthLine_;

	Player* pPlayer_;
};

#include "CodeBox.h"
#include <cassert>
#include "Player.h"
#include "Vector2Int.h"
#include "Utility/VectorUtil.h"
#include "Screen.h"


namespace
{
	static const int FRAME_MARGIN{ 6 };  // フレームの余白
	static const int TEXT_MARGIN{ 10 };  // テキストまでの余白
	static const int LINE_MARGIN{ 5 };  // 行の幅
	static const int PLAYER_HEAD_MARGIN{ 30 };  // プレイヤー頭からの余白
	static const int FRAME_OFFSET{ 100 };
}

CodeBox::CodeBox() :
	sourceMaxLengthLine_{ -1 },
	sourceMaxLength_{ 0 }
{
	pPlayer_ = FindGameObject<Player>();
	assert(pPlayer_ != nullptr
		&& "プレイヤーが見つからなかった @CodeBox::Draw");
}

CodeBox::~CodeBox()
{
}

void CodeBox::Update()
{
}

void CodeBox::Draw()
{
	PositionStyle posStyle{ PS_PLAYER };
	// 枠の矩形 (フレームなし)
	RectInt drawBox{ GetDrawRect(posStyle) };
	if (drawBox.y < 0)
	{
		posStyle = PS_BOTTOM;
		drawBox = GetDrawRect(posStyle);
	}
	else if (drawBox.y + drawBox.height > Screen::HEIGHT)
	{
		posStyle = PS_TOP;
		drawBox = GetDrawRect(posStyle);
	}

	// フレームの描画
	DrawBox(
		drawBox.x - FRAME_MARGIN, drawBox.y - FRAME_MARGIN,
		drawBox.x + drawBox.width + FRAME_MARGIN, drawBox.y + drawBox.height + FRAME_MARGIN,
		0x000000, TRUE);
	// 背景四角の描画
	DrawBox(
		drawBox.x, drawBox.y,
		drawBox.x + drawBox.width, drawBox.y + drawBox.height,
		0xffffff, TRUE);

	// テキストの描画
	Vector2Int textBegin{ GetDrawTextBegin(posStyle) };
	if (textBegin.y < 0)
	{
		textBegin.y += pPlayer_->GetHeight() + PLAYER_HEAD_MARGIN;
	}
	for (int line = 0; line < sourceLines_.size(); line++)
	{
		Vector2Int draw{ textBegin.x, textBegin.y + (GetFontSize() + LINE_MARGIN) * line };
		if (pPlayer_->GetReadLine() == line)
		{
			DrawBox(
				draw.x, draw.y,
				draw.x + GetTextBoxWidth(), draw.y + GetFontSize(),
				0xff0000, TRUE);
			DrawFormatString(
				draw.x, draw.y,
				0xffffff,
				"%s", sourceLines_[line].c_str());
		}
		else
		{
			DrawFormatString(
				draw.x, draw.y,
				0x000000,
				"%s", sourceLines_[line].c_str());
		}
	}
}

void CodeBox::SetSourceLines(const std::vector<std::string>& _lines)
{
	sourceLines_.clear();
	sourceMaxLengthLine_ = -1;

	VectorUtil::CopyVector(sourceLines_, _lines);

	if (sourceLines_.size() <= 0)
	{
		return;
	}
	int maxLine{ 0 };  // 最初は1行目が最大だと仮定

	for (int line = 1; line < _lines.size(); line++)
	{
		if (_lines[maxLine].size() < _lines[line].size())
		{
			maxLine = line;
		}
	}
	sourceMaxLength_ = _lines[maxLine].size();
	sourceMaxLengthLine_ = maxLine;
}

int CodeBox::GetTextBoxWidth() const
{
	if (sourceMaxLengthLine_ <= -1)
	{
		return 0;  // テキストが読み込まれていないなら幅は 0
	}
	
	// テキスト占有の横幅
	int textWidth = GetDrawFormatStringWidth("%s", sourceLines_[sourceMaxLengthLine_].c_str());

	return textWidth;
}

int CodeBox::GetTextBoxHeight() const
{
	if (sourceMaxLengthLine_ <= -1)
	{
		return 0;  // テキストが読み込まれていないなら幅は 0
	}

	int fontHeight = GetFontSize();
	int textHeight = fontHeight * sourceLines_.size();  // テキスト占有の高さ
	textHeight += LINE_MARGIN * sourceLines_.size() - 1;

	return textHeight;
}

Vector2Int CodeBox::GetDrawTextBegin(const PositionStyle _positionStyle) const
{
	Vector2Int playerHead = pPlayer_->ToWorld({ 0, -pPlayer_->GetHeight() / 2.0f }).ToInt();
	int beginX
	{
		playerHead.x  // プレイヤーの頭
		- GetTextBoxWidth() / 2  // テキストボックスの半分を引く
	};
	int beginY{};

	switch (_positionStyle)
	{
	case PS_TOP:
		beginY = FRAME_OFFSET;
		break;
	case PS_BOTTOM:
		beginY = Screen::HEIGHT - GetTextBoxHeight() - FRAME_OFFSET;
		break;
	case PS_PLAYER:
		beginY =
			playerHead.y  // プレイヤーの頭
			- PLAYER_HEAD_MARGIN  // プレイヤーの頭とボックスの余白
			- GetTextBoxHeight(); // テキストボックス
		break;
	default:
		break;
	}

	return Vector2Int{ beginX, beginY };
}

RectInt CodeBox::GetDrawRect(const PositionStyle _positionStyle) const
{
	// テキストの余白も考慮
	RectInt rect{};
	rect.pivot = GetDrawTextBegin(_positionStyle) - Vector2Int{ TEXT_MARGIN, TEXT_MARGIN };
	rect.width = GetTextBoxWidth() + TEXT_MARGIN;
	rect.height = GetTextBoxHeight() + TEXT_MARGIN;

	return rect;
}

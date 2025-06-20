#include "CodeBox.h"
#include <cassert>
#include "Player.h"
#include "Vector2Int.h"


namespace
{
	static const int FRAME_MARGIN{ 6 };  // フレームの余白
	static const int TEXT_MARGIN{ 10 };  // テキストまでの余白
	static const int LINE_MARGIN{ 5 };  // 行の幅
}

CodeBox::CodeBox()
{
}

CodeBox::~CodeBox()
{
}

void CodeBox::Update()
{
}

void CodeBox::Draw()
{
	Player* pPlayer = FindGameObject<Player>();
	assert(pPlayer != nullptr
		&& "プレイヤーが見つからなかった @CodeBox::Draw");

	Vector2 player = pPlayer->ToWorld({ 0, 0 });

	DrawCircle(player.x, player.y, 10, 0xffff, TRUE);

	Rectan drawBox{};

	Vector2Int boxSize{ GetBoxWidth(), GetBoxHeight() };

	drawBox.x = static_cast<float>(-boxSize.x) / 2;
	drawBox.y = static_cast<float>(-boxSize.y) / 2;
	drawBox.pivot = boxSize / -2;
	//drawBox.width

	DrawBox();
}

void CodeBox::SetSourceLines(const std::vector<std::string>& _lines)
{
	sourceLines_ = _lines;
	int max{ 0 };
	for (auto& line : _lines)
	{
		if (max > line.size())
		{
			max = line.size();
		}
	}
	sourceMaxLength_ = max;
}

int CodeBox::GetBoxWidth() const
{
	int margin = FRAME_MARGIN + TEXT_MARGIN;  // 対称マージン

	int fontWidth = GetFontMaxWidth();
	int textWidth = sourceMaxLength_ * fontWidth;  // テキスト占有の横幅

	return margin + textWidth;
}

int CodeBox::GetBoxHeight() const
{
	int margin = FRAME_MARGIN + TEXT_MARGIN;  // 対称マージン
	margin += TEXT_MARGIN * (sourceLines_.size() - 1);

	int fontHeight = GetFontSize();
	int textHeight = sourceLines_.size() * fontHeight;  // テキスト占有の高さ

	return margin + textHeight;
}

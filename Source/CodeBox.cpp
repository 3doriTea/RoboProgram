#include "CodeBox.h"
#include <cassert>
#include "Player.h"
#include "Vector2Int.h"


namespace
{
	static const int FRAME_MARGIN{ 6 };  // �t���[���̗]��
	static const int TEXT_MARGIN{ 10 };  // �e�L�X�g�܂ł̗]��
	static const int LINE_MARGIN{ 5 };  // �s�̕�
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
		&& "�v���C���[��������Ȃ����� @CodeBox::Draw");

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
	int margin = FRAME_MARGIN + TEXT_MARGIN;  // �Ώ̃}�[�W��

	int fontWidth = GetFontMaxWidth();
	int textWidth = sourceMaxLength_ * fontWidth;  // �e�L�X�g��L�̉���

	return margin + textWidth;
}

int CodeBox::GetBoxHeight() const
{
	int margin = FRAME_MARGIN + TEXT_MARGIN;  // �Ώ̃}�[�W��
	margin += TEXT_MARGIN * (sourceLines_.size() - 1);

	int fontHeight = GetFontSize();
	int textHeight = sourceLines_.size() * fontHeight;  // �e�L�X�g��L�̍���

	return margin + textHeight;
}

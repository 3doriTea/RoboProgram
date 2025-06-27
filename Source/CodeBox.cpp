#include "CodeBox.h"
#include <cassert>
#include "Player.h"
#include "Vector2Int.h"
#include "Utility/VectorUtil.h"
#include "Utility/RectanUtility.h"
#include "Screen.h"


namespace
{
	static const int FRAME_MARGIN{ 6 };  // �t���[���̗]��
	static const int TEXT_MARGIN{ 10 };  // �e�L�X�g�܂ł̗]��
	static const int LINE_MARGIN{ 5 };  // �s�̕�
	static const int PLAYER_HEAD_MARGIN{ 30 };  // �v���C���[������̗]��
	static const int FRAME_OFFSET{ 100 };
	static const int PLAYER_SIDE_MARGIN{ 300 };  // �v���C���[���̗]��
}

CodeBox::CodeBox() :
	sourceMaxLengthLine_{ -1 },
	sourceMaxLength_{ 0 }
{
	pPlayer_ = FindGameObject<Player>();
	assert(pPlayer_ != nullptr
		&& "�v���C���[��������Ȃ����� @CodeBox::Draw");
}

CodeBox::~CodeBox()
{
}

void CodeBox::Update()
{
}

void CodeBox::Draw()
{
	PositionStyleY posStyleY{ PSY_PLAYER };
	PositionStyleX posStyleX{ PSX_PLAYER };
	// �g�̋�` (�t���[���Ȃ�)
	RectInt drawBox{ GetDrawRect(posStyleY, posStyleX) };
	if (drawBox.y < 0)
	{
		posStyleY = PSY_BOTTOM;
		drawBox = GetDrawRect(posStyleY, posStyleX);
	}
	else if (drawBox.y + drawBox.height > Screen::HEIGHT)
	{
		posStyleY = PSY_TOP;
		drawBox = GetDrawRect(posStyleY, posStyleX);
	}

	RectInt playerRect{ pPlayer_->GetRect().ToInt() };

	if (RectanUtility::IsHit(drawBox, playerRect))
	{
		posStyleX = PSX_PLAYER_RIGHT;
		drawBox = GetDrawRect(posStyleY, posStyleX);
	}

	// �t���[���̕`��
	DrawBox(
		drawBox.x - FRAME_MARGIN, drawBox.y - FRAME_MARGIN,
		drawBox.x + drawBox.width + FRAME_MARGIN, drawBox.y + drawBox.height + FRAME_MARGIN,
		0x000000, TRUE);
	// �w�i�l�p�̕`��
	DrawBox(
		drawBox.x, drawBox.y,
		drawBox.x + drawBox.width, drawBox.y + drawBox.height,
		0xffffff, TRUE);

	// �e�L�X�g�̕`��
	Vector2Int textBegin{ GetDrawTextBegin(posStyleY, posStyleX) };
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

	/*DEBUG_BOX
	DrawBox(drawBox.x, drawBox.y, drawBox.x + drawBox.width, drawBox.y + drawBox.height, 0x00ff00, TRUE);
	DrawBox(playerRect.x, playerRect.y, playerRect.x + playerRect.width, playerRect.y + playerRect.height, 0x0000ff, TRUE);*/
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
	int maxLine{ 0 };  // �ŏ���1�s�ڂ��ő傾�Ɖ���

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

void CodeBox::SetByteCodes(const std::vector<Byte>& _byteCode)
{
	byteCodes_ = _byteCode;
}

int CodeBox::GetTextBoxWidth() const
{
	if (sourceMaxLengthLine_ <= -1)
	{
		return 0;  // �e�L�X�g���ǂݍ��܂�Ă��Ȃ��Ȃ畝�� 0
	}
	
	// �e�L�X�g��L�̉���
	int textWidth = GetDrawFormatStringWidth("%s", sourceLines_[sourceMaxLengthLine_].c_str());

	return textWidth;
}

int CodeBox::GetTextBoxHeight() const
{
	if (sourceMaxLengthLine_ <= -1)
	{
		return 0;  // �e�L�X�g���ǂݍ��܂�Ă��Ȃ��Ȃ畝�� 0
	}

	size_t maxLinesCount
	{
		byteCodes_.size() > sourceLines_.size()
		? byteCodes_.size()
		: sourceLines_.size()
	};

	int fontHeight = GetFontSize();
	int textHeight = fontHeight * maxLinesCount;  // �e�L�X�g��L�̍���
	textHeight += LINE_MARGIN;// * sourceLines_.size() - 1;

	return textHeight;
}

Vector2Int CodeBox::GetDrawTextBegin(
	const PositionStyleY _posStyleY,
	const PositionStyleX _posStyleX) const
{
	Vector2Int playerHead = pPlayer_->ToWorld({ 0, -pPlayer_->GetHeight() / 2.0f }).ToInt();

	int beginX{};

	switch (_posStyleX)
	{
	case PSX_PLAYER:
		beginX = playerHead.x  // �v���C���[�̓�
			- GetTextBoxWidth() / 2;  // �e�L�X�g�{�b�N�X�̔���������
		break;
	case PSX_PLAYER_RIGHT:
		beginX = playerHead.x  // �v���C���[�̓�
			+ pPlayer_->GetWidth()  // �v���C���[�̌���
			+ PLAYER_SIDE_MARGIN;  // �]��
		break;
	default:
		break;
	}

	int beginY{};

	switch (_posStyleY)
	{
	case PSY_TOP:
		beginY = FRAME_OFFSET;
		break;
	case PSY_BOTTOM:
		beginY = Screen::HEIGHT - GetTextBoxHeight() - FRAME_OFFSET;
		break;
	case PSY_PLAYER:
		beginY =
			playerHead.y  // �v���C���[�̓�
			- PLAYER_HEAD_MARGIN  // �v���C���[�̓��ƃ{�b�N�X�̗]��
			- GetTextBoxHeight(); // �e�L�X�g�{�b�N�X
		break;
	default:
		break;
	}

	return Vector2Int{ beginX, beginY };
}

RectInt CodeBox::GetDrawRect(const PositionStyleY _posStyleY, const PositionStyleX _posStyleX) const
{
	// �e�L�X�g�̗]�����l��
	RectInt rect{};
	rect.pivot = GetDrawTextBegin(_posStyleY, _posStyleX) - Vector2Int{ TEXT_MARGIN, TEXT_MARGIN };
	rect.width = GetTextBoxWidth() + TEXT_MARGIN;
	rect.height = GetTextBoxHeight() + TEXT_MARGIN;

	return rect;
}

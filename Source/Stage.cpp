#include "Stage.h"
#include "IO/Input.h"
#include "../Library/CsvReader/CsvReader.h"
#include <cassert>
#include "Player.h"
#include "Coin.h"
#include "Flag.h"
#include "Doc.h"
#include "Lightning.h"


namespace
{
	static const int TILE_WIDTH{ 80 };
	static const int TILE_HEIGHT{ 80 };
	static const float INVALED_POSITION{ -100 };
	static const char* TILE_FILES[]
	{
		"",
		"Data/Image/tileGround1.png",
		"",  // coin
		"",  // player
		"",  // flag
		"",  // doc
		"Data/Image/Tile1.png",  // tile1
	};
}

Stage::Stage()
{
	int hImage = LoadGraph(TILE_FILES[TILE_GROUND]);
	assert(hImage > 0
		&& "�摜�ǂݍ��݂Ɏ��s @Stage::Stage");
	hImages_.insert({ TILE_GROUND, hImage });

	hImage = LoadGraph(TILE_FILES[TILE_1]);
	assert(hImage > 0
		&& "�摜�ǂݍ��݂Ɏ��s @Stage::Stage");
	hImages_.insert({ TILE_1, hImage });

	CsvReader* csv{ new CsvReader{ "Data/map.csv" } };
	
	// CSV����X�e�[�W�f�[�^��ǂݍ���
	for (int line = 0; line < csv->GetLines(); line++)
	{
		std::vector<Tile> mapLine{};
		for (int column = 0; column < csv->GetColumns(line); column++)
		{
			mapLine.push_back((Tile)csv->GetInt(line, column));
		}
		map_.push_back(mapLine);
	}

	delete csv;

	// �`�F�b�N�|�C���g�̏�񂪂Ȃ�
	if (checkPoint_.x <= INVALED_POSITION + 1
		&& checkPoint_.y <= INVALED_POSITION + 1)
	{
		Vector2 playerPosition{};
		assert(TryFindPlayerPositionFromMap(&playerPosition)
			&& "�}�b�v�f�[�^�Ƀv���C���[�����Ȃ�");

		pPlayer_ = new Player{ playerPosition };
	}
	else
	{
		pPlayer_ = new Player{ checkPoint_ };
	}

	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			switch (map_[y][x])
			{
			case TILE_COIN:
				new Coin
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			case TILE_FLAG:
				new Flag
				{
					{
						static_cast<float>(x)* TILE_WIDTH,
						static_cast<float>(y)* TILE_HEIGHT
					}
				};
				break;
			case TILE_DOC:
				new Doc
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			case TILE_LIGHTNING:
				new Lightning
				{
					{
						static_cast<float>(x) * TILE_WIDTH,
						static_cast<float>(y) * TILE_HEIGHT
					}
				};
				break;
			default:
				break;
			}
		}
	}
}

Stage::~Stage()
{
	for (auto& hImage : hImages_)
	{
		DeleteGraph(hImage.second);
	}
}

//void Stage::Update()
//{
//}

void Stage::Draw()
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			position_.x = x * TILE_WIDTH;
			position_.y = y * TILE_HEIGHT;

			int tileType = map_[y][x];

			switch (tileType)
			{
			case TILE_GROUND:
				DrawTile({ x, y }, TILE_GROUND);
				break;
			case TILE_1:
				DrawTile({ x, y }, TILE_1);
				break;
			default:
				break;
			}

		}
	}
}

#pragma region �����蔻��
float Stage::CheckRight(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };
	return _position.x - tileWorldPosition.x + 1;  // �[����0�̏ꍇ��1����
}

float Stage::CheckLeft(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };

	return TILE_WIDTH - (_position.x - tileWorldPosition.x);
}

float Stage::CheckBottom(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };
	return _position.y - tileWorldPosition.y + 1;  // ���������[����1�����Ă���
}

float Stage::CheckTop(Vector2 _position)
{
	if (IsWall(_position) == false)
	{
		return 0.0f;
	}

	Vector2 tileWorldPosition{ ToWorldPosition(ToTilePosition(_position)) };

	return TILE_HEIGHT - (_position.y - tileWorldPosition.y);
}

bool Stage::IsWall(const Vector2& _position)
{
	Vector2 tilePosition{ ToTilePosition(_position) };

	// �z��̊O�ɍs����������Ȃ� ��������͉����Ȃ����瓖�����ĂȂ������
	if (tilePosition.y < 0.0f || tilePosition.y >= map_.size())
	{
		return false;
	}
	// �������z��̊O�̏��� TODO: �ǂ̊O�ɂ����Ȃ��悤�ɂ������Ȃ�A������true�Ԃ�
	if (tilePosition.x < 0.0f || tilePosition.x >= map_[tilePosition.y].size())
	{
		return false;
	}

	// �^�C���̔ԍ������āA�ǂ��ǂ����m�肷��
	switch (map_[tilePosition.y][tilePosition.x])
	{
	case TILE_NONE:  // ��
	case TILE_PLAYER:  // �v���C���[
	case TILE_COIN:  // �R�C��
	case TILE_FLAG:  // �`�F�b�N�|�C���g
	case TILE_DOC:  // �`�F�b�N�|�C���g
	case TILE_LIGHTNING:  // ���d
	case TILE_MAX:  // ?
		return false;  // �ǂłȂ�
	default:
		break;
	}
	// �l��Ԃ��֐��������ƒl��Ԃ��Ă��邩��ۏႷ�邽�߂ɁA���₷���̂���
	return true;  // �ǂł���
}
#pragma endregion

#pragma region �ϊ��n
Vector2 Stage::ToWorldPosition(const Vector2& tilePosition)
{
	return
	{
		tilePosition.x * TILE_WIDTH,
		tilePosition.y * TILE_HEIGHT,
	};
}

Vector2 Stage::ToTilePosition(const Vector2& worldPosition)
{
	return
	{  // �[�������
		static_cast<float>(static_cast<int>(worldPosition.x / TILE_WIDTH)),
		static_cast<float>(static_cast<int>(worldPosition.y / TILE_HEIGHT)),
	};
}
const int Stage::GetTile(const Vector2Int& tilePosition) const
{
	int tileX{ tilePosition.x };
	int tileY{ tilePosition.y };

	// �͈͊O���O
	if (tileY < 0 || map_.size() <= tileY)
	{
		return -1;
	}
	if (tileX < 0 || map_[tileY].size() <= tileX)
	{
		return -1;
	}

	printfDx("�`�F�b�N %d,%d block:%d\n", tileX, tileY, map_[tileY][tileX]);

	return map_[tileY][tileX];
}
#pragma endregion

const Vector2Int Stage::GetPush(const Vector2Int& _point, const PushDir _dir) const
{
	int tileX{ _point.x / TILE_WIDTH };
	int tileY{ _point.y / TILE_HEIGHT };

	// �͈͊O���O
	if (tileY < 0 || map_.size() <= tileY)
	{
		return Vector2Int::Zero();
	}
	if (tileX < 0 || map_[tileY].size() <= tileX)
	{
		return Vector2Int::Zero();
	}

	// �^�C�����ǂł��邩
	Tile tile = map_[tileY][tileX];
	switch (tile)
	{
	case TILE_GROUND:
		break;
	default:
		return Vector2Int::Zero();  // �ǂł͂Ȃ��Ȃ��A
	}

	Vector2Int push{};

	switch (_dir)
	{
	case PushDir::Up:
		push.y = TILE_HEIGHT - (_point.y % TILE_HEIGHT) + 1;
		break;
	case PushDir::Down:
		push.y = -(_point.y % TILE_HEIGHT) - 1;
		break;
	case PushDir::Left:
		push.x = TILE_WIDTH - (_point.x % TILE_WIDTH) + 1;
		break;
	case PushDir::Right:
		push.x = -(_point.x % TILE_WIDTH) - 1;
		break;
	default:
		break;
	}
	return push;
}

const Vector2Int Stage::GetPush(const RectInt& _rect) const
{
	Vector2Int push{};
	GetPush({}, PushDir::Up);

	return Vector2Int();
}

void Stage::DrawTile(const Vector2Int& _tilePosition, const Tile& _tileId)
{
	int w{ TILE_WIDTH };
	int h{ TILE_HEIGHT };

	DrawGraph(
		_tilePosition.x * w - scroll_.x,
		_tilePosition.y * h - scroll_.y,
		hImages_[_tileId], TRUE);
	/*DrawRectGraph(
		_tilePosition.x * w - scroll_.x, _tilePosition.y * h - scroll_.y,
		TILE_WIDTH * w, TILE_HEIGHT * h,
		w, h,
		hImages_[_tileId], TRUE);*/
}

bool Stage::TryFindPlayerPositionFromMap(Vector2* _pPosition) const
{
	for (int y = 0; y < map_.size(); y++)
	{
		for (int x = 0; x < map_[y].size(); x++)
		{
			if (map_[y][x] == TILE_PLAYER)
			{
				*_pPosition = Vector2(
					x * TILE_WIDTH + TILE_WIDTH / 2.0f,
					y * TILE_HEIGHT + TILE_HEIGHT / 2.0f);
				return true;
			}
		}
	}

	return false;
}

Player* Stage::GetPlayer()
{
	assert(pPlayer_ != nullptr);
	return pPlayer_;
}

void Stage::SetCheckPoint(const Vector2& _point)
{
	checkPoint_ = _point;
}

Vector2 Stage::GetCheckPoint() const
{
	return checkPoint_;
}

Vector2 Stage::checkPoint_{ INVALED_POSITION, INVALED_POSITION };

#include "Stage.h"
#include "IO/Input.h"
#include "../Library/CsvReader/CsvReader.h"


namespace
{
	static const int TILE_WIDTH{ 80 };
	static const int TILE_HEIGHT{ 80 };
}

Stage::Stage() :
	map_{ new CsvReader{ "Data/map.csv" } }
{
	for (int y = 0; y < map_->GetLines(); y++)
	{
		for (int x = 0; x < map_->GetColumns(y); x++)
		{
			switch (map_->GetInt(y, x))
			{
			case TILE_PLAYER:
				break;
			default:
				break;
			}
		}
	}
}

Stage::~Stage()
{
	delete map_;
}

void Stage::Update()
{
}

void Stage::Draw()
{
}

const Vector2Int Stage::GetPush(const Vector2Int& _point, const PushDir _dir) const
{
	int tileX{ _point.x / TILE_WIDTH };
	int tileY{ _point.y / TILE_HEIGHT };

	// 範囲外除外
	if (tileY < 0 || map_->GetLines() <= tileY)
	{
		return Vector2Int::Zero();
	}
	if (tileX < 0 || map_->GetColumns(tileY) <= tileX)
	{
		return Vector2Int::Zero();
	}

	// タイルが壁であるか
	int tile = map_->GetInt(tileY, tileX);
	switch (tile)
	{
	case TILE_GROUND:
		break;
	default:
		return Vector2Int::Zero();  // 壁ではないなら回帰
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


	return Vector2Int();
}

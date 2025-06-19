#pragma once
#include "../Library/GameObject.h"
#include "Vector2Int.h"
#include "RectInt.h"

class CsvReader;

class Stage : public GameObject
{
private:
	enum struct PushDir
	{
		Up,
		Down,
		Left,
		Right,
	};
	enum Tile
	{
		TILE_NONE,
		TILE_GROUND,
		TILE_COIN,
		TILE_PLAYER,

		TILE_MAX,
	};
public:
	Stage();
	~Stage();

	void Update() override;
	void Draw() override;

	const Vector2Int GetPush(const Vector2Int& _point, const PushDir _dir) const;
	const Vector2Int GetPush(const RectInt& _rect) const;

private:
	CsvReader* map_;
};

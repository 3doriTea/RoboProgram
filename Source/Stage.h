#pragma once
#include "Object2D.h"
#include "Vector2Int.h"
#include "RectInt.h"
#include "Vector2.h"
#include <vector>
#include <map>
#include "IO/FileSaver.h"


class CsvReader;
class Player;

class Stage : public Object2D
{
private:
	enum struct PushDir
	{
		Up,
		Down,
		Left,
		Right,
	};
public:
	enum Tile
	{
		TILE_NONE = 0,
		TILE_GROUND = 1,
		TILE_COIN = 2,
		TILE_PLAYER = 3,
		TILE_FLAG = 4,
		TILE_DOC = 5,
		TILE_1 = 6,
		TILE_INFO = 7,
		TILE_GOAL = 8,  // 博士

		TILE_LIGHTNING = 9,

		TILE_MAX,
	};
public:
	Stage();
	~Stage();

	//void Update() override;
	void Draw() override;

#pragma region 当たり判定
	/// <summary>
	/// 指定した座標が当たっているか調べる
	/// </summary>
	/// <param name="position">調べる座標</param>
	/// <returns>当たっていれば左に押し出す量</returns>
	float CheckRight(Vector2 position);
	/// <summary>
	/// 指定した座標が当たっているか調べる
	/// </summary>
	/// <param name="position">調べる座標</param>
	/// <returns>当たっていれば右に押し出す量</returns>
	float CheckLeft(Vector2 position);
	/// <summary>
	/// 指定した座標が当たっているか調べる
	/// </summary>
	/// <param name="position">調べる座標</param>
	/// <returns>当たっていれば上に押し出す量</returns>
	float CheckBottom(Vector2 position);
	/// <summary>
	/// 指定した座標が当たっているか調べる
	/// </summary>
	/// <param name="position">調べる座標</param>
	/// <returns>当たっていれば下に押し出す量</returns>
	float CheckTop(Vector2 position);

	bool IsWall(const Vector2& position);
#pragma endregion

#pragma region 変換系
	/// <summary>
		/// ワールド座標をタイル座標に変換する
		/// </summary>
		/// <param name="tilePosition">タイル座標</param>
		/// <returns>ワールド座標</returns>
	Vector2 ToWorldPosition(const Vector2& tilePosition);
	Vector2 ToWorldPosition(const Vector2Int& tilePosition);
	/// <summary>
	/// タイル座標をワールド座標に変換する
	/// </summary>
	/// <param name="worldPosition">ワールド座標</param>
	/// <returns>タイル座標</returns>
	Vector2 ToTilePosition(const Vector2& worldPosition);
#pragma endregion
	const int GetTile(const Vector2Int& tilePosition) const;

	const Vector2Int GetPush(const Vector2Int& _point, const PushDir _dir) const;
	const Vector2Int GetPush(const RectInt& _rect) const;

	/// <summary>
	/// タイルを描画する
	/// </summary>
	/// <param name="_tilePosition">描画するタイルの座標</param>
	/// <param name="_tileId">画像のポイント</param>
	void DrawTile(
		const Vector2Int& _tilePosition,
		const Tile& _tileId);

	inline Vector2 GetScroll() const { return scroll_; }
	inline void SetScroll(const Vector2& _scroll) { scroll_ = _scroll; }

	bool TryFindPlayerPositionFromMap(Vector2* _pPosition) const;

	Player* GetPlayer();

	/// <summary>
	/// チェックポイント
	/// </summary>
	/// <param name="_point">新しくチェックポイント</param>
	void SetCheckPoint(const Vector2& _point);
	/// <summary>
	/// チェックポイントを取得
	/// </summary>
	/// <returns>チェックポイントの座標を取得</returns>
	Vector2 GetCheckPoint() const;

	/// <summary>
	/// ドキュメントを開く
	/// </summary>
	void OpenDocument();

	/// <summary>
	/// ドキュメントレベルを上げる
	/// </summary>
	void GetDocument(const int _docLevel);

	void GetInfo(const int _infoLevel);

	/// <summary>
	/// ゴールする
	/// </summary>
	void DoGoal() { isGoaled_ = true; }

	/// <summary>
	/// 状況をセーブする
	/// </summary>
	void Save();

	/// <summary>
	/// 現在のドキュメントレベルを取得する
	/// </summary>
	/// <returns>ドキュメントレベル</returns>
	inline const int GetDocLevel() const { return documentLevel_; }
	/// <summary>
	/// 現在の情報レベルを取得する
	/// </summary>
	/// <returns></returns>
	inline const int GetInfoLevel() const { return infoLevel_; }

private:
	std::string WriteDocument() const;

#pragma region セーブデータ
	Vector2 checkPoint_;  // チェックポイント
	int documentLevel_;  // ドキュメントレベル
	int infoLevel_;      // 情報レベル
	bool isGoaled_;   // ゴールした true / false
#pragma endregion

	Vector2 scroll_;  // スクロール量
	Vector2 position_;  // 座標
	Player* pPlayer_;

	std::vector<std::vector<Tile>> map_;
	std::map<Tile, int> hImages_;

	FileSaver saveFile_;
};

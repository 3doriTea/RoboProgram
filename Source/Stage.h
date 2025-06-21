#pragma once
#include "Object2D.h"
#include "Vector2Int.h"
#include "RectInt.h"
#include "Vector2.h"
#include <vector>
#include <map>


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
		TILE_NONE,
		TILE_GROUND,
		TILE_COIN,
		TILE_PLAYER,

		TILE_MAX,
	};
public:
	Stage();
	~Stage();

	//void Update() override;
	void Draw() override;

#pragma region �����蔻��
	/// <summary>
	/// �w�肵�����W���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="position">���ׂ���W</param>
	/// <returns>�������Ă���΍��ɉ����o����</returns>
	float CheckRight(Vector2 position);
	/// <summary>
	/// �w�肵�����W���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="position">���ׂ���W</param>
	/// <returns>�������Ă���ΉE�ɉ����o����</returns>
	float CheckLeft(Vector2 position);
	/// <summary>
	/// �w�肵�����W���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="position">���ׂ���W</param>
	/// <returns>�������Ă���Ώ�ɉ����o����</returns>
	float CheckBottom(Vector2 position);
	/// <summary>
	/// �w�肵�����W���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="position">���ׂ���W</param>
	/// <returns>�������Ă���Ή��ɉ����o����</returns>
	float CheckTop(Vector2 position);

	bool IsWall(const Vector2& position);
#pragma endregion

#pragma region �ϊ��n
	/// <summary>
		/// ���[���h���W���^�C�����W�ɕϊ�����
		/// </summary>
		/// <param name="tilePosition">�^�C�����W</param>
		/// <returns>���[���h���W</returns>
	Vector2 ToWorldPosition(const Vector2& tilePosition);
	/// <summary>
	/// �^�C�����W�����[���h���W�ɕϊ�����
	/// </summary>
	/// <param name="worldPosition">���[���h���W</param>
	/// <returns>�^�C�����W</returns>
	Vector2 ToTilePosition(const Vector2& worldPosition);
#pragma endregion

	const Vector2Int GetPush(const Vector2Int& _point, const PushDir _dir) const;
	const Vector2Int GetPush(const RectInt& _rect) const;

	/// <summary>
	/// �^�C����`�悷��
	/// </summary>
	/// <param name="_tilePosition">�`�悷��^�C���̍��W</param>
	/// <param name="_tileId">�摜�̃|�C���g</param>
	void DrawTile(
		const Vector2Int& _tilePosition,
		const Tile& _tileId);

	inline Vector2 GetScroll() const { return scroll_; }
	inline void SetScroll(const Vector2& _scroll) { scroll_ = _scroll; }

	bool TryFindPlayerPositionFromMap(Vector2* _pPosition) const;

	Player* GetPlayer() { return pPlayer_; }

private:
	Vector2 scroll_;  // �X�N���[����
	Vector2 position_;  // ���W
	Player* pPlayer_;

	std::vector<std::vector<Tile>> map_;
	std::map<Tile, int> hImages_;
};

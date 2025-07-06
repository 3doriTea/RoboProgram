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
		TILE_GOAL = 8,  // ���m

		TILE_LIGHTNING = 9,

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
	Vector2 ToWorldPosition(const Vector2Int& tilePosition);
	/// <summary>
	/// �^�C�����W�����[���h���W�ɕϊ�����
	/// </summary>
	/// <param name="worldPosition">���[���h���W</param>
	/// <returns>�^�C�����W</returns>
	Vector2 ToTilePosition(const Vector2& worldPosition);
#pragma endregion
	const int GetTile(const Vector2Int& tilePosition) const;

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

	Player* GetPlayer();

	/// <summary>
	/// �`�F�b�N�|�C���g
	/// </summary>
	/// <param name="_point">�V�����`�F�b�N�|�C���g</param>
	void SetCheckPoint(const Vector2& _point);
	/// <summary>
	/// �`�F�b�N�|�C���g���擾
	/// </summary>
	/// <returns>�`�F�b�N�|�C���g�̍��W���擾</returns>
	Vector2 GetCheckPoint() const;

	/// <summary>
	/// �h�L�������g���J��
	/// </summary>
	void OpenDocument();

	/// <summary>
	/// �h�L�������g���x�����グ��
	/// </summary>
	void GetDocument(const int _docLevel);

	void GetInfo(const int _infoLevel);

	/// <summary>
	/// �S�[������
	/// </summary>
	void DoGoal() { isGoaled_ = true; }

	/// <summary>
	/// �󋵂��Z�[�u����
	/// </summary>
	void Save();

	/// <summary>
	/// ���݂̃h�L�������g���x�����擾����
	/// </summary>
	/// <returns>�h�L�������g���x��</returns>
	inline const int GetDocLevel() const { return documentLevel_; }
	/// <summary>
	/// ���݂̏�񃌃x�����擾����
	/// </summary>
	/// <returns></returns>
	inline const int GetInfoLevel() const { return infoLevel_; }

private:
	std::string WriteDocument() const;

#pragma region �Z�[�u�f�[�^
	Vector2 checkPoint_;  // �`�F�b�N�|�C���g
	int documentLevel_;  // �h�L�������g���x��
	int infoLevel_;      // ��񃌃x��
	bool isGoaled_;   // �S�[������ true / false
#pragma endregion

	Vector2 scroll_;  // �X�N���[����
	Vector2 position_;  // ���W
	Player* pPlayer_;

	std::vector<std::vector<Tile>> map_;
	std::map<Tile, int> hImages_;

	FileSaver saveFile_;
};

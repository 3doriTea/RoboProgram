#pragma once
#include "Object2D.h"
#include <vector>
#include <string>


class Player;

class CodeBox : public Object2D
{
public:
	CodeBox();
	~CodeBox();

	void Update() override;
	void Draw() override;

	void SetSourceLines(
		const std::vector<std::string>& _lines);

private:
	enum PositionStyle
	{
		PS_PLAYER,
		PS_TOP,
		PS_BOTTOM,
	};

	/// <summary>
	/// �e�L�X�g�{�b�N�X�̉������擾
	/// </summary>
	/// <returns>pixel</returns>
	int GetTextBoxWidth() const;
	/// <summary>
	/// �e�L�X�g�{�b�N�X�̍������擾
	/// </summary>
	/// <returns>pixel</returns>
	int GetTextBoxHeight() const;
	/// <summary>
	/// �e�L�X�g�{�b�N�X�̕`��J�n�_���擾
	/// </summary>
	/// <returns>�`����W</returns>
	Vector2Int GetDrawTextBegin(const PositionStyle _positionStyle) const;
	/// <summary>
	/// �t���[���Ȃ��̕`���`
	/// </summary>
	/// <returns></returns>
	RectInt GetDrawRect(const PositionStyle _positionStyle) const;

	std::vector<std::string> sourceLines_;
	int sourceMaxLength_;  // TODO: �����ő啝�Ŏ��s������A�ő�̍s�ԍ���ۑ�����A�������畝�v�Z
	int sourceMaxLengthLine_;

	Player* pPlayer_;
};

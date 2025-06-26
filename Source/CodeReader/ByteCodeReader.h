#pragma once
#include <vector>


using Byte = unsigned char;

/// <summary>
/// �o�C�g�R�[�h��ǂݎ�邽�߂̃X�g���[���N���X
/// </summary>
class ByteCodeReader
{
public:
	enum struct SeekPoint
	{
		Head,
		Tail,
	};

	ByteCodeReader(const std::vector<Byte>& _byteCode, const size_t _current = 0);
	~ByteCodeReader();

	/// <summary>
	/// �R�[�h�̏I�[�ɓ��B���Ă��邩
	/// </summary>
	/// <returns>���B���Ă��� true / false</returns>
	bool IsEndOfCode() const;
	/// <summary>
	/// �����ƈ�v������i�߂�A��v���Ȃ�������i�߂Ȃ�
	/// </summary>
	/// <param name="_code">��r�R�[�h</param>
	/// <returns>��v���� true / false</returns>
	bool Consume(const Byte _code);
	/// <summary>
	/// �P���ɏo�͂��Đi�߂�
	/// </summary>
	/// <returns>���݌��Ă���</returns>
	Byte Pop();

	/// <summary>
	/// ���݂̃C���f�b�N�X���擾
	/// </summary>
	/// <returns>���݌��Ă���R�[�h�̃C���f�b�N�X</returns>
	size_t GetCurrentIndex() const { return current_; }

	/// <summary>
	/// ���݌��Ă���C���f�b�N�X���w�肷��
	/// </summary>
	/// <param name="_index"></param>
	void Seek(const size_t _index);
	/// <summary>
	/// ���݌��Ă���C���f�b�N�X�𑊑ΓI�Ɏw�肷��
	/// </summary>
	/// <param name="_seekPoint"></param>
	void SeekTo(const SeekPoint _seekPoint);

private:
	const std::vector<Byte>& byteCode_;  // �ǂݎ��o�C�g�R�[�h
	size_t current_;  // ���݂̈ʒu
};

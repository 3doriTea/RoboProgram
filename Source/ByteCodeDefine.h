#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
#pragma region ���{�b�g����
	// �������Ȃ�
	BCD_NOP = 0x00,  // ()
	// ��~
	BCD_HALT = 0x76,  // ()
	// �A�N�V��������
	BCD_ACT = 0xCB,  // (�A�N�V�����ԍ�)
#pragma endregion
	
#pragma region �R�[�h�̈�
	// �֐��Ăяo��
	BCD_CALL = 0xCD,  // (�֐�-���΃R�[�h�A�h���X)
	// �֐��ԋp
	BCD_RET = 0xC9,  // () �X�^�b�N�}�V���ɕԋp�l�����Ă���
	// �W�����v
	//BCD_JP = 0xC3,  // (�W�����v��-��΃R�[�h�A�h���X)
#pragma endregion

#pragma region �f�[�^�̈�
	// �f�[�^�̈�ɒl���Z�b�g����
	BCD_DSET = 0xDB,  // (�Z�b�g��-��΃f�[�^�A�h���X, �Z�b�g�����΃��W�X�^�A�h���X)
	// �f�[�^�̈悩��l���R�s�[����
	BCD_DGET,  // (�Q�b�g��-��΃f�[�^�A�h���X, �R�s�[���΃��W�X�^�A�h���X)
#pragma endregion

#pragma region �X�^�b�N�̈�
	// �X�^�b�N�}�V���Ƀv�b�V������
	BCD_PUS = 0xC5,  // (�R�s�[�����W�X�^)
	BCD_PUSW,        // (�R�s�[�����W�X�^, ���W�X�^�̎g�p�T�C�Y)

	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_ADD = 0x6B,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_SUB,
	// �X�^�b�N�}�V���Ńg�b�v2����Z�A�X�^�b�N�}�V���ɓ����
	BCD_MUL,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_DIV,

	// �X�^�b�N�}�V���Ƀ|�b�v����
	BCD_POP = 0xC1,  // (�o�͐惌�W�X�^)
	BCD_POPW,        // (�o�͐惌�W�X�^, ���W�X�^�̎g�p�T�C�Y)
#pragma endregion
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // �������Ȃ�
	BCD_ACT_RUN = 0x01,  // ����
	BCD_ACT_JUMP = 0x02,  // �W�����v
};

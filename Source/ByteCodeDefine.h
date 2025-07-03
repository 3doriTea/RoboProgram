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
	BCD_ACT = 0x77,  // (�A�N�V�����ԍ�)

	BCD_AIO = 0xCC,
#pragma endregion
#pragma region �R�[�h�̈�
	// �֐��Ăяo��
	BCD_CALL = 0xCD,  // (�֐�-���΃R�[�h�A�h���X) 2�o�C�g�ڊ
	// �֐��ԋp
	BCD_RET = 0xC9,  // () �X�^�b�N�}�V���ɕԋp�l�����Ă���
	// �W�����v
	BCD_JMP = 0xA0,  // (�W�����v��-���΃R�[�h�A�h���X) 2�o�C�g�ڊ
	// ���� false �̂Ƃ��W�����v
	BCD_CFJP = 0xCA,  // (��r-���W�X�^�A�h���X, �W�����v��-���΃R�[�h�A�h���X) 3�o�C�g�ڊ
	// ���� true �̂Ƃ��W�����v
	BCD_CTJP = 0xCB,  // (��r-���W�X�^�A�h���X, �W�����v��-���΃R�[�h�A�h���X) 3�o�C�g�ڊ
#pragma endregion
#pragma region �f�[�^�̈�
	// �f�[�^�̈�ɒl���Z�b�g����
	BCD_DSET = 0xDB,  // (�Z�b�g��-��΃f�[�^�A�h���X, �Z�b�g�����΃��W�X�^�A�h���X)
	// �f�[�^�̈悩��l���R�s�[����
	BCD_DGET = 0xDC,  // (�Q�b�g��-��΃f�[�^�A�h���X, �R�s�[���΃��W�X�^�A�h���X)
#pragma endregion
#pragma region ���W�X�^�𑀍삷����
	BCD_RSET = 0x3A,  // (�Z�b�g��-���W�X�^�A�h���X, �Z�b�g���鑦�l)
#pragma endregion
#pragma region �X�^�b�N�̈�
	// �X�^�b�N�}�V���Ƀv�b�V������
	BCD_PUS = 0xC5,   // (�R�s�[�����W�X�^)
	BCD_PUSW = 0xC6,  // (�R�s�[�����W�X�^, ���W�X�^�̎g�p�T�C�Y)
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_ADD = 0x6B,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_SUB = 0x6C,
	// �X�^�b�N�}�V���Ńg�b�v2����Z�A�X�^�b�N�}�V���ɓ����
	BCD_MUL = 0x6D,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_DIV = 0x6E,

	BCD_AND = 0x7A,
	BCD_OR = 0x7B,
	BCD_EQUAL = 0x7C,
	BCD_NOTEQUAL = 0x7D,
	BCD_LESSTHAN = 0x7E,  // <
	BCD_GREATERTHEN = 0x7F,  // >
	BCD_LESSEQUAL = 0x8A,  // <=
	BCD_GREATEREQUAL = 0x8B,  // >=

	// �X�^�b�N�}�V���Ƀ|�b�v����
	BCD_POP = 0xC1,  // (�o�͐惌�W�X�^)
	BCD_POPW = 0xC2,        // (�o�͐惌�W�X�^, ���W�X�^�̎g�p�T�C�Y)
#pragma endregion
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // �������Ȃ�
	BCD_ACT_RUN = 0x01,  // ����
	BCD_ACT_JUMP = 0x02,  // �W�����v
};

enum ByteCodeDefine_AIO : Byte
{
	BCD_AIO_ISGROUND = 0x01,
	BCD_AIO_CHECKTILE = 0x02,
};

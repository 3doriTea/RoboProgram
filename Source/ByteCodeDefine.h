#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
	// �������Ȃ�
	BCD_NOP = 0x00,  // ()
	// ��~
	BCD_HALT = 0x76,  // ()
	// �A�N�V��������
	BCD_ACT = 0xCB,  // (�A�N�V�����ԍ�)
	
	// �֐��Ăяo��
	BCD_CALL = 0xCD,  // (�֐��̃|�C���^)
	// �֐��ԋp
	BCD_RET = 0xC9,  // () �X�^�b�N�}�V���ɕԋp�l������
	// �W�����v
	BCD_JP = 0xC3,  // ()

	// �X�^�b�N�}�V���Ƀv�b�V������
	BCD_PUSH = 0xC5,  // (�R�s�[���A�h���X)

	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_ADD = 0x6B,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_SUB,
	// �X�^�b�N�}�V���Ńg�b�v2����Z�A�X�^�b�N�}�V���ɓ����
	BCD_MUL,
	// �X�^�b�N�}�V���Ńg�b�v2�����Z�A�X�^�b�N�}�V���ɓ����
	BCD_DIV,

	// �X�^�b�N�}�V���Ƀ|�b�v����
	BCD_POP = 0xC1,  // (�ړ���A�h���X)
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // �������Ȃ�
	BCD_ACT_RUN = 0x01,  // ����
	BCD_ACT_JUMP = 0x02,  // �W�����v
};

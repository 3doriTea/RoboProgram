#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
	BCD_NOP = 0x00,  // �������Ȃ�
	// ...
	BCD_HALT = 0x76,  // ��~
	// ...
	BCD_ACT = 0xCB,  // �A�N�V����
	// ...
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // �������Ȃ�
	BCD_ACT_RUN = 0x01,  // ����
	BCD_ACT_JUMP = 0x02,  // �W�����v
};

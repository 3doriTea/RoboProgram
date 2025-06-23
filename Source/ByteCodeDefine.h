#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
	BCD_NOP = 0x00,  // 何もしない
	// ...
	BCD_HALT = 0x76,  // 停止
	// ...
	BCD_ACT = 0xCB,  // アクション
	// ...
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // 何もしない
	BCD_ACT_RUN = 0x01,  // 走る
	BCD_ACT_JUMP = 0x02,  // ジャンプ
};

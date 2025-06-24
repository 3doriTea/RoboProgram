#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
	// 何もしない
	BCD_NOP = 0x00,  // ()
	// 停止
	BCD_HALT = 0x76,  // ()
	// アクションする
	BCD_ACT = 0xCB,  // (アクション番号)
	
	// 関数呼び出し
	BCD_CALL = 0xCD,  // (関数のポインタ)
	// 関数返却
	BCD_RET = 0xC9,  // () スタックマシンに返却値を入れる
	// ジャンプ
	BCD_JP = 0xC3,  // ()

	// スタックマシンにプッシュする
	BCD_PUSH = 0xC5,  // (アドレス)

	// 
	BCD_ADD = 0x6B,

	// スタックマシンにポップする
	BCD_POP = 0xC1,  // (代入先アドレス)
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // 何もしない
	BCD_ACT_RUN = 0x01,  // 走る
	BCD_ACT_JUMP = 0x02,  // ジャンプ

};

#pragma once

using Byte = unsigned char;

enum ByteCodeDefine : Byte
{
#pragma region ロボット制御
	// 何もしない
	BCD_NOP = 0x00,  // ()
	// 停止
	BCD_HALT = 0x76,  // ()
	// アクションする
	BCD_ACT = 0x77,  // (アクション番号)

	BCD_AIO = 0xCC,
#pragma endregion
#pragma region コード領域
	// 関数呼び出し
	BCD_CALL = 0xCD,  // (関数-相対コードアドレス) 2バイト目基準
	// 関数返却
	BCD_RET = 0xC9,  // () スタックマシンに返却値を入れてある
	// ジャンプ
	BCD_JMP = 0xA0,  // (ジャンプ先-相対コードアドレス) 2バイト目基準
	// 条件 false のときジャンプ
	BCD_CFJP = 0xCA,  // (比較-レジスタアドレス, ジャンプ先-相対コードアドレス) 3バイト目基準
	// 条件 true のときジャンプ
	BCD_CTJP = 0xCB,  // (比較-レジスタアドレス, ジャンプ先-相対コードアドレス) 3バイト目基準
#pragma endregion
#pragma region データ領域
	// データ領域に値をセットする
	BCD_DSET = 0xDB,  // (セット先-絶対データアドレス, セットする絶対レジスタアドレス)
	// データ領域から値をコピーする
	BCD_DGET = 0xDC,  // (ゲット元-絶対データアドレス, コピー先絶対レジスタアドレス)
#pragma endregion
#pragma region レジスタを操作するやつ
	BCD_RSET = 0x3A,  // (セット先-レジスタアドレス, セットする即値)
#pragma endregion
#pragma region スタック領域
	// スタックマシンにプッシュする
	BCD_PUS = 0xC5,   // (コピー元レジスタ)
	BCD_PUSW = 0xC6,  // (コピー元レジスタ, レジスタの使用サイズ)
	// スタックマシンでトップ2つを加算、スタックマシンに入れる
	BCD_ADD = 0x6B,
	// スタックマシンでトップ2つを減算、スタックマシンに入れる
	BCD_SUB = 0x6C,
	// スタックマシンでトップ2つを乗算、スタックマシンに入れる
	BCD_MUL = 0x6D,
	// スタックマシンでトップ2つを除算、スタックマシンに入れる
	BCD_DIV = 0x6E,

	BCD_AND = 0x7A,
	BCD_OR = 0x7B,
	BCD_EQUAL = 0x7C,
	BCD_NOTEQUAL = 0x7D,
	BCD_LESSTHAN = 0x7E,  // <
	BCD_GREATERTHEN = 0x7F,  // >
	BCD_LESSEQUAL = 0x8A,  // <=
	BCD_GREATEREQUAL = 0x8B,  // >=

	// スタックマシンにポップする
	BCD_POP = 0xC1,  // (出力先レジスタ)
	BCD_POPW = 0xC2,        // (出力先レジスタ, レジスタの使用サイズ)
#pragma endregion
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // 何もしない
	BCD_ACT_RUN = 0x01,  // 走る
	BCD_ACT_JUMP = 0x02,  // ジャンプ
};

enum ByteCodeDefine_AIO : Byte
{
	BCD_AIO_ISGROUND = 0x01,
	BCD_AIO_CHECKTILE = 0x02,
};

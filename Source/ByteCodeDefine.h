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
	BCD_ACT = 0xCB,  // (アクション番号)
#pragma endregion
	
#pragma region コード領域
	// 関数呼び出し
	BCD_CALL = 0xCD,  // (関数-相対コードアドレス) 2バイト目基準
	// 関数返却
	BCD_RET = 0xC9,  // () スタックマシンに返却値を入れてある
	// ジャンプ
	//BCD_JP = 0xC3,  // (ジャンプ先-絶対コードアドレス)
	// 条件 false のときジャンプ
	BCD_CFJP,  // (比較-レジスタアドレス, ジャンプ先-相対コードアドレス) 3バイト目基準
	// 条件 true のときジャンプ
	BCD_CTJP,  // (比較-レジスタアドレス, ジャンプ先-相対コードアドレス) 3バイト目基準
#pragma endregion

#pragma region データ領域
	// データ領域に値をセットする
	BCD_DSET = 0xDB,  // (セット先-絶対データアドレス, セットする絶対レジスタアドレス)
	// データ領域から値をコピーする
	BCD_DGET,  // (ゲット元-絶対データアドレス, コピー先絶対レジスタアドレス)
#pragma endregion

#pragma region レジスタを操作するやつ
	BCD_RSET = 0x3A,  // (セット先-レジスタアドレス, セットする即値)
#pragma endregion

#pragma region スタック領域
	// スタックマシンにプッシュする
	BCD_PUS = 0xC5,  // (コピー元レジスタ)
	BCD_PUSW,        // (コピー元レジスタ, レジスタの使用サイズ)


	// スタックマシンでトップ2つを加算、スタックマシンに入れる
	BCD_ADD = 0x6B,
	// スタックマシンでトップ2つを減算、スタックマシンに入れる
	BCD_SUB,
	// スタックマシンでトップ2つを乗算、スタックマシンに入れる
	BCD_MUL,
	// スタックマシンでトップ2つを除算、スタックマシンに入れる
	BCD_DIV,

	BCD_AND,
	BCD_OR,
	BCD_EQUAL,
	BCD_NOTEQUAL,
	BCD_LESSTHAN,  // <
	BCD_GREATERTHEN,  // >
	BCD_LESSEQUAL,  // <=
	BCD_GREATEREQUAL,  // >=

	// スタックマシンにポップする
	BCD_POP = 0xC1,  // (出力先レジスタ)
	BCD_POPW,        // (出力先レジスタ, レジスタの使用サイズ)
#pragma endregion
};

enum ByteCodeDefine_ACT : Byte
{
	BCD_ACT_NONE = 0x00,  // 何もしない
	BCD_ACT_RUN = 0x01,  // 走る
	BCD_ACT_JUMP = 0x02,  // ジャンプ
};

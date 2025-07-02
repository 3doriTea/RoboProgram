#include "SemanticAnalyzer.h"
#include <cassert>
#include <DxLib.h>
#include "../ByteCodeDefine.h"

static const size_t DEBUG_STR_SIZE{ 64 };

char debugString[DEBUG_STR_SIZE]{};

#define PRINTF(...) \
::sprintf_s<64>(debugString, __VA_ARGS__); \
OutputDebugString(debugString);

void SemanticAnalyzer::Analyze()
{
	if (in_.first.size() <= 0)
	{
		Error("トークン木がないよ！");
		return;
	}
	for (auto& nodes : in_.first)
	{
		if (nodes->type_ == NODE_GLOBAL)
		{
			Read(nodes, 0);
			return;
		}
	}

	Error("グローバルがないよ！");
}

int SemanticAnalyzer::NewMemory(std::string name, int size)
{
	assert(size == 4 && "4byte以外の型サイズは未実装です。");

	std::vector<bool> used(maxMemorySize / 4, false);

	int offset = 0;
	for (auto& mem : memory)
	{
		used[mem.second.offset / 4] = true;
		offset++;
	}

	for (int i = 0; i < used.size(); i++)
	{
		// 使われていない場所発見
		if (used[i] == false)
		{
			memory.insert({ name, UseMemory{ offset * 4, 4 }});
			return i * 4;
		}
	}
	// もし空き場所ないなら

	memory.insert({ name, UseMemory{ offset * 4, 4 } });
	int add = maxMemorySize;
	maxMemorySize += size;

	return add;
}

int SemanticAnalyzer::GetMemory(const std::string& name)
{
	return memory[name].offset;
}

void SemanticAnalyzer::RegSet(const int value, ByteCodes& bc, int offset)
{
	Byte* val{ reinterpret_cast<Byte*>(value) };
	for (int i = 0; i < sizeof(int); i++)
	{
		bc.push_back({ {}, BCD_RSET });
		bc.push_back({ {}, static_cast<Byte>(offset + i) });
		bc.push_back({ {}, val[i] });
	}
}

void SemanticAnalyzer::RegSet(const Byte value, ByteCodes& bc, int offset)
{
	bc.push_back({ {}, BCD_RSET });
	bc.push_back({ {}, static_cast<Byte>(offset) });
	bc.push_back({ {}, value });
}

void SemanticAnalyzer::Read(const NODE* n, const int _depth, int position)
{
	assert(n != nullptr
		&& "どこかで null った！");

	int d = _depth + 1;
	switch (n->type_)
	{
	case NODE_PROC:
		PRINTF("プロセス\n");
		Read(n->proc.proc, d);
		if (n->proc.next != nullptr)  // 次のプロセスがあるなら
		{
			Read(n->proc.next, d);
		}
		break;// 処理
	case NODE_EXPR:
		PRINTF("式\n");
		Read(n->expr.ls, d);
		Read(n->expr.rs, d);
		break;// 式
	case NODE_OR:
		Read(n->expr.rs, d);
		PRINTF("or\n");
		Read(n->expr.ls, d);
		break;// ||
	case NODE_AND:
		Read(n->expr.rs, d);
		PRINTF("and\n");
		Read(n->expr.ls, d);
		break;// &&
	case NODE_EQUAL:
		Read(n->expr.ls, d);
		PRINTF("==\n");
		Read(n->expr.rs, d);
		break;// ==
	case NODE_NOTEQUAL:
		Read(n->expr.ls, d);
		PRINTF("!=\n");
		Read(n->expr.rs, d);
		break;// !=
	case NODE_LESSTHAN:
		Read(n->expr.ls, d);
		PRINTF("<\n");
		Read(n->expr.rs, d);
		break;// <
	case NODE_GREATERTHEN:
		Read(n->expr.ls, d);
		PRINTF(">\n");
		Read(n->expr.rs, d);
		break;// >
	case NODE_LESSEQUAL:
		Read(n->expr.ls, d);
		PRINTF("<=\n");
		Read(n->expr.rs, d);
		break;// <=
	case NODE_GREATEREQUAL:
		Read(n->expr.ls, d);
		PRINTF(">=\n");
		Read(n->expr.rs, d);
		break;// >=
	case NODE_ADD:
		
		Read(n->expr.ls, d);
		PRINTF("+\n");
		Read(n->expr.rs, d);
		break;// +
	case NODE_SUB:
		Read(n->expr.ls, d);
		PRINTF("-\n");
		Read(n->expr.rs, d);
		break;// -
	case NODE_MUL:
		Read(n->expr.ls, d);
		PRINTF("*\n");
		Read(n->expr.rs, d);
		break;// *
	case NODE_DIV:
		Read(n->expr.ls, d);
		PRINTF("/\n");
		Read(n->expr.rs, d);
		break;// /
	case NODE_NFOR:
		PRINTF("for\n");
		Read(n->nfor.init, d);
		Read(n->nfor.expr, d);
		Read(n->nfor.updt, d);
		Read(n->nfor.proc, d);
		break;// for文
	case NODE_NIF:
		PRINTF("if\n");
		Read(n->nif.expr, d);
		Read(n->nif.proc, d);
		break;// if文
	case NODE_VARDEC:
		PRINTF("変数宣言\n");
		Read(n->varDec.type, d);
		Read(n->varDec.assigns, d);
		break;// 変数宣言
	case NODE_FUNCDEC:
		PRINTF("関数定義\n");		
		ReadFuncDec(n, position);
		break;// 関数宣言
	//case NODE_VALUE:
		//PRINTF("値:%s\n", in_.second[n->tokenIndex_].second.c_str());
		
		//break;// 値
	case NODE_INTEGER:
		PRINTF("%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 整数値
	//case NODE_LITER_DIGIT:
	//	PRINTF("\n");
	//	break;// 整数リテラル
	case NODE_CALLFUNC:
		PRINTF("関数呼び出し:\n");
		Read(n->callFunc.name, d);
		if (n->callFunc.args != nullptr)  // 引数があるなら
		{
			Read(n->callFunc.args, d);
		}
		break;// 関数呼び出し
	case NODE_INCREMENT:
		PRINTF("インクリメント:\n");
		Read(n->expr.ls, d);
		break;// ++
	case NODE_DECREMENT:
		PRINTF("デクリメント:\n");
		Read(n->expr.ls, d); 
		break;// --
	case NODE_NAME:
		PRINTF("名前:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 名
	case NODE_TYPE:
		PRINTF("型:%s\n", in_.second[n->tokenIndex_].second.c_str());
		break;// 型
	case NODE_RET:
		PRINTF("返却:\n");
		Read(n->ret.expr, d);
		break;// return
	case NODE_ARG:
		PRINTF("実引数:\n");
		Read(n->arg.expr, d);
		if (n->arg.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->arg.next, d);
		}

		break;// 実引数
	case NODE_ASSIGN:
		PRINTF("代入:\n");
		Read(n->assigns.name, d);
		PRINTF("=:\n");
		Read(n->assigns.expr, d);
		if (n->assigns.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->assigns.next, d);
		}
		break;// 代入式
	case NODE_PARAM:
		PRINTF("仮引数\n");
		Read(n->param.varDec, d);
		if (n->param.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->param.next, d);
		}
		break;// 仮引数
	case NODE_REGISTER_FUNC_NAME:
		PRINTF("登録済み関数名:\n");
		Read(n->func.name, d);
		break;
	case NODE_REGISTER_VAR_NAME:
		PRINTF("登録済み変数名:\n");
		Read(n->var.name, d);
		break;
	case NODE_GLOBAL:
		PRINTF("グローバル\n");
		Read(n->global.funcDef, d);
		if (n->global.next != nullptr)
		{
			PRINTF(",\n");
			Read(n->global.next, d);
		}
		break;
	default:
		assert(false && "処理しきれていない");
	}
}

void SemanticAnalyzer::ReadFuncDec(const NODE* n, int position)
{
	std::string funcName = ReadName(n->funcDec.name);
	funcGroup.insert({ funcName, {} });

	FuncData& data{ funcGroup[funcName] };
	ByteCodes& bc{ data.byteCodes };
	bc.offset = position;
}

std::string SemanticAnalyzer::ReadName(const NODE* n)
{
	assert(n->type_ == NODE_NAME);

	assert(n != nullptr && "nullptrだった！ @SemanticAnalyzer::ReadName");
	
	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_が範囲外だった @SemanticAnalyzer::ReadName");

	return in_.second[n->tokenIndex_].second;
}

int SemanticAnalyzer::ReadType(const NODE* n)
{
	assert(n->type_ == NODE_TYPE);

	assert(n != nullptr && "nullptrだった！ @SemanticAnalyzer::ReadType");

	assert(0 <= n->tokenIndex_ && n->tokenIndex_ <= in_.second.size()
		&& "tokenIndex_が範囲外だった @SemanticAnalyzer::ReadType");

	std::string typeName{ in_.second[n->tokenIndex_].second };

	if (typeName == "void")
		return 0;
	else if (typeName == "int")
		return 4;
	else
		return -1;
}

int SemanticAnalyzer::ReadInteger(const NODE* n)
{
	assert(n != nullptr && "nullptrだった！ @SemanticAnalyzer::ReadName");

	std::string str{ in_.second[n->tokenIndex_].second };

	return std::stoi(str);
}

void SemanticAnalyzer::ReadParam(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_PARAM);

	std::string varName = ReadName(n->param.varDec->varDec.assigns->assigns.name);

	int typeSize{ ReadType(n->param.varDec->varDec.type) };

	if (typeSize == 0)  // 型のサイズが 0 なら void 指定されている
	{
		Error("関数の仮引数に void型は使用できません。");
		return;
	}

	assert(typeSize == 4 && "未対応の型サイズ:4byte以外の型が指定された");

	// TODO: int型だと仮確定している
	// スタックから取り出し、4byte分レジスタに格納する
	bc.push_back({ {}, BCD_POPW });
	bc.push_back({ {}, 4 });

	int addr = NewMemory(varName, 4);
	MemorySet(addr, 4, bc);

	// MEMO: 実引数とは逆で、仮引数は前から順に処理していく
	if (n->param.next != nullptr)
	{
		ReadParam(n->param.next, bc);
	}
}

void SemanticAnalyzer::ReadProcs(const NODE* n, ByteCodes& bc, int begin)
{
	assert(n->type_ == NODE_PROC);

	// 処理は以下の分岐
	switch (n->proc.proc->type_)
	{
	case NODE_NFOR:
		ReadNFor(n->proc.proc, bc, -1);
		break;
	case NODE_NIF:
		ReadNIf(n->proc.proc, bc, -1);
		break;
	case NODE_ASSIGN:
		ReadAssign(n->proc.proc, bc, -1);
		break;
	case NODE_VARDEC:
		ReadVarDec(n->proc.proc, bc);
		break;
	case NODE_RET:
		ReadRet(n->proc.proc, bc);
		break;
	default:
		assert(false && "例外処理が呼ばれた");
		break;
	}

	// MEMO: 逐次的に処理していく
	if (n->proc.next != nullptr)  // 次の処理があるなら
	{
		ReadProcs(n->proc.next, bc, begin);
	}
}

void SemanticAnalyzer::ReadRet(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_RET);

	// 戻り値の式を処理する、処理後はスタックにプッシュされているため、そのまま返却する
	ReadExpr(n->ret.expr, bc);
}

void SemanticAnalyzer::ReadArg(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_ARG);

	// MEMO: 引数の後ろを優先してスタックにプッシュしていくことで、読み取るときは前から読み取れる
	if (n->arg.next != nullptr)
	{
		ReadArg(n->arg.next, bc);
	}

	ReadExpr(n->arg.expr, bc);  // 式を書いてもらう
}

void SemanticAnalyzer::ReadNFor(const NODE* n, ByteCodes& bc, int blockBegin)
{
	assert(n->type_ == NODE_NFOR);

	ReadAssign(n->nfor.init, bc, -1);

	ByteCodes forBlock{};
	ReadProcs(n->nfor.proc, forBlock, blockBegin + bc.size());

	ReadExpr(n->nfor.expr, forBlock);

	// ifの条件処理書き終えたら ifのブロック書く
	for (auto&& byteCode : forBlock)
	{
		bc.push_back(byteCode);
	}

	bc.push_back({ {}, BCD_CFJP });
	bc.push_back({ {}, 0 });

	assert(-bc.size() >= INT8_MIN);

	bc.push_back({ {}, -bc.size() });
}

void SemanticAnalyzer::ReadNIf(const NODE* n, ByteCodes& bc, int blockBegin)
{
	assert(n->type_ == NODE_NIF);

	ReadExpr(n->nif.expr, bc);

	ByteCodes ifBlock{};
	ReadProcs(n->nif.proc, ifBlock, blockBegin + bc.size());

	// TODO: ifのところのジャンプミスっていたらここが原因
	bc.push_back({ {}, BCD_CFJP });  // falseのときジャンプ
	bc.push_back({ {}, 0 });  // 0番レジスタ を参照

	assert(ifBlock.size() <= INT8_MAX);

	bc.push_back({ {}, static_cast<Byte>(ifBlock.size()) });  // ブロックプロセス分先にジャンプ

	// ifの条件処理書き終えたら ifのブロック書く
	for (auto&& byteCode : ifBlock)
	{
		bc.push_back(byteCode);
	}
}

void SemanticAnalyzer::ReadExpr(const NODE* n, ByteCodes& bc)
{
	// オペランド(被演算子) だったら
	switch (n->type_)
	{
	case NODE_REGISTER_VAR_NAME:
		ReadVar(n->var, bc);  // 変数を参照する
		break;
	case NODE_REGISTER_FUNC_NAME:
		ReadCallFunc(n->func, bc);
		break;
	case NODE_INTEGER:
	{
		int val{ ReadInteger(n) };
		RegSet(val, bc, 0);
		bc.push_back({ {}, BCD_PUSW });
		bc.push_back({ {}, 0 });
		bc.push_back({ {}, 4 });
		break;
	}
	default:
		break;
	}

	// オペレータ(演算子) だったら
	switch (n->type_)
	{
	case NODE_ADD:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_ADD });
		break;
	case NODE_SUB:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_SUB });
		break;
	case NODE_MUL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_MUL });
		break;
	case NODE_DIV:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_DIV });
		break;
	case NODE_AND:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_AND });
		break;
	case NODE_OR:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_OR });
		break;
	case NODE_EQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_EQUAL });
		break;
	case NODE_NOTEQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_NOTEQUAL });
		break;
	case NODE_LESSTHAN:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_LESSTHAN });
		break;
	case NODE_GREATERTHEN:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATERTHEN });
		break;
	case NODE_LESSEQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_LESSEQUAL });
		break;
	case NODE_GREATEREQUAL:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATEREQUAL });
		break;
	case NODE_INCREMENT:
		ReadExpr(n->expr.ls, bc);
		ReadExpr(n->expr.rs, bc);
		bc.push_back({ {}, BCD_GREATEREQUAL });
		break;
	default:
		assert(false && "計算できない演算子");
		break;
	}
}

void SemanticAnalyzer::ReadAssign(const NODE* n, ByteCodes& bc, const int typeSize)
{
	// TODO: 型サイズ4byte以外になる場合、連続した宣言時に型サイズも引数で渡す必要がある
	assert(n->type_ == NODE_ASSIGN);

	// TODO: もしここ今くいかないなら name の前に何かかんでいるかも
	std::string varName = ReadName(n->assigns.name);
	int addr{ -1 };

	if (typeSize > 0)  // サイズが 0 以上 = 確定している
	{
		addr = NewMemory(varName, typeSize);
	
		assert(addr >= 0 && "メモリの確保ができなかった。");
	}
	else  // サイズが 0 =	 不確定 = 変数参照してあげる
	{
		addr = GetMemory(varName);

		if (addr < 0)
		{
			Error("宣言されていない変数が参照されました。");
			return;
		}
	}


	// 代入式があるなら処理する
	if (n->assigns.expr != nullptr)
	{
		// 式を処理、処理後はスタックに値が残っている
		ReadExpr(n->assigns.expr, bc);

		// TODO: int型だと仮確定している
		// スタックから取り出し、4byte分レジスタに格納する
		bc.push_back({ {}, BCD_POPW });
		bc.push_back({ {}, typeSize });
		MemorySet(addr, typeSize, bc);
	}

	// 連続宣言があるなら処理する
	if (n->assigns.next)
	{
		ReadAssign(n->assigns.next, bc, typeSize);
	}
}

void SemanticAnalyzer::ReadVar(const NODE* n, ByteCodes& bc)
{
	assert(n->type_ == NODE_REGISTER_VAR_NAME);

	std::string varName{ ReadName(n->var.name) };

	int addr{ GetMemory(varName) };

	if (addr < 0)
	{
		Error("宣言されていない変数が参照されました。");
		return;
	}

	// TODO: 型サイズが4だと仮確定している

	//bc.push_back({ {}, BCD_DGET });
	MemoryGet(addr, 4, bc);  // メモリからレジスタに書き写す

	// レジスタからスタックにプッシュする
	bc.push_back({ {}, BCD_PUSW });
	bc.push_back({ {}, 0 });
	bc.push_back({ {}, 4 });
}

void SemanticAnalyzer::ReadVarDec(const NODE* n, ByteCodes& bc, bool allowInit, bool consecutive)
{
	// TODO: 型サイズ4byte以外になる場合、連続した宣言時に型サイズも引数で渡す必要がある
	assert(n->type_ == NODE_VARDEC);


	if (allowInit == false)
	{
		// 初期化許可されていないのに式が入っている
		if (n->varDec.assigns->assigns.expr != nullptr)
		{
			Error("初期化できません。");
			return;
		}
	}

	if (consecutive == false)
	{
		// 連続した変数宣言が許可されていないのに、連続している
		if (n->varDec.assigns->assigns.next != nullptr)
		{
			Error("連続で変数宣言はできません。");
			return;
		}
	}


	int typeSize{ ReadType(n->param.varDec->varDec.type) };

	if (typeSize == 0)  // 型のサイズが 0 なら void 指定されている
	{
		Error("変数宣言として void型は使用できません。");
		return;
	}

	assert(typeSize == 4 && "未対応の型サイズ:4byte以外の型が指定された");

	ReadAssign(n->varDec.assigns, bc, typeSize);  // 変数、代入とかを処理
}

void SemanticAnalyzer::ReadCallFunc(const NODE* n, ByteCodes& bc)
{
	std::string funcName = ReadName(n->callFunc.name);
	if (funcGroup.count(funcName) != 1)
	{
		Error(std::string("関数:") + funcName + "は未定義です。");
		return;
	}

	if (funcGroup[funcName].index < 0)
	{
		Error(std::string("関数:") + funcName + "は未実装です。");
		return;
	}

	// NOTE: 関数呼び出しの位置ずれていたらここが原因
	int current{ bc.offset + bc.size() };
	int diff{ funcGroup[funcName].index - current };  // 呼び出し関数の相対位置を求める

	assert(INT8_MIN <= diff && diff <= INT8_MAX
		&& "関数の位置が8bitで表せる範囲外になってしまった");

	// 関数の位置は決まった

	// 実引数をセットする
	ReadArg(n->callFunc.args, bc);

	bc.push_back({ {}, BCD_CALL });
	bc.push_back({ {}, static_cast<Byte>(diff) });
}

void SemanticAnalyzer::MemorySet(const int _addr, const int _size, ByteCodes& bc)
{
	// メモリの _addr からレジスタの _size バイト分を書き込む
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DSET });  // 命令
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // セット先-メモリ
		bc.push_back({ {}, static_cast<Byte>(i) });  // レジスタ指定
	}
}

void SemanticAnalyzer::MemoryGet(const int _addr, const int _size, ByteCodes& bc)
{
	// メモリの _addr からレジスタに _size バイト分を読み込む
	for (int i = 0; i < _size; i++)
	{
		bc.push_back({ {}, BCD_DGET });   // 命令
		bc.push_back({ {}, static_cast<Byte>(_addr + i) });  // ゲット先-メモリ
		bc.push_back({ {}, static_cast<Byte>(i) });  // レジスタ指定
	}
}

//void SemanticAnalyzer::StackPush(const int _regOffset, const int _size, ByteCodes& bc)
//{
//	for (int i = 0; i < _size; i++)
//	{
//		bc.push_back({ {}, BCD_PUSW });  // 命令
//		bc.push_back({ {}, _regOffset + i });
//		bc.push_back({ {}, _size });
//	}
//}

void SemanticAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, {});
}

void SemanticAnalyzer::Error(const std::string& _message)
{
	Error(_message.c_str());
}

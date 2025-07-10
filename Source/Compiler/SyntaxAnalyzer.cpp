#include "SyntaxAnalyzer.h"
#include <cassert>

void SyntaxAnalyzer::Analyze()
{
	for (readIndex_ = 0; readIndex_ < in_.size(); readIndex_++)
	{
		_Global();
		if (HasError()) return;
	}
}

NODE* SyntaxAnalyzer::_Global()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (readIndex_ >= in_.size())
	{
		return nullptr;
	}

	return NewNode({ GetIdx(), NODE_GLOBAL, _FuncDef(), _Global() });
}

NODE* SyntaxAnalyzer::_Expr()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	return _Lor();
}

#pragma region スタック解決する関数

NODE* SyntaxAnalyzer::_Lor()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Land() };

	while (true)
	{
		if (Consume("||"))
		{
			node = NewNode({ GetIdx(), NODE_OR, node, _Land() });
		}
		else
		{
			return node;
		}
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Land()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Equal() };

	while (true)
	{
		if (Consume("&&"))
		{
			node = NewNode({ GetIdx(), NODE_AND, node, _Equal() });
		}
		else
		{
			return node;
		}
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Equal()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Then() };

	while (true)
	{
		if (Consume("=="))
		{
			node = NewNode({ GetIdx(), NODE_EQUAL, node, _Then() });
		}
		else if (Consume("!="))
		{
			node = NewNode({ GetIdx(), NODE_NOTEQUAL, node, _Then() });
		}
		else
		{
			return node;
		}
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Then()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Add() };

	while (true)
	{
		if (Consume("<"))
			node = NewNode({ GetIdx(), NODE_LESSTHAN, node, _Add() });
		else if (Consume(">"))
			node = NewNode({ GetIdx(), NODE_GREATERTHEN, node, _Add() });
		else if (Consume("<="))
			node = NewNode({ GetIdx(), NODE_LESSEQUAL, node, _Add() });
		else if (Consume(">="))
			node = NewNode({ GetIdx(), NODE_GREATEREQUAL, node, _Add() });
		else
			return node;
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Add()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Mul() };

	while (true)
	{
		if (Consume("+"))
		{
			node = NewNode({ GetIdx(), NODE_ADD, node, _Mul() });
		}
		else if (Consume("-"))
		{
			node = NewNode({ GetIdx(), NODE_SUB, node, _Mul() });
		}
		else
		{
			return node;
		}
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Mul()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Unary() };

	while (true)
	{
		if (Consume("*"))
		{
			node = NewNode({ GetIdx(), NODE_MUL, node, _Unary() });
		}
		else if (Consume("/"))
		{
			node = NewNode({ GetIdx(), NODE_DIV, node, _Unary() });
		}
		else
		{
			return node;
		}
		// エラーがあるならreturn
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Unary()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Consume("+"))
	{
		return _Postfix();
	}
	if (Consume("-"))
	{
		return NewNode({ GetIdx(), NODE_SUB, nullptr, _Postfix() });
	}
	return _Postfix();
}

NODE* SyntaxAnalyzer::_Postfix()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Primary() };

	if (Consume("++"))
	{
		node = NewNode({ GetIdx(), NODE_INCREMENT, node });
	}
	else if (Consume("--"))
	{
		node = NewNode({ GetIdx(), NODE_DECREMENT, node });
	}

	return node;
}

NODE* SyntaxAnalyzer::_Primary()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Consume("("))
	{
		NODE* node{ _Expr() };
		Expect(")");
		return node;
	}

	return _Val();
}

NODE* SyntaxAnalyzer::_Val()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Liter() };
	if (node == nullptr)
	{
		node = _CallFunc();
	}
	if (node == nullptr)
	{
		node = _Var();
	}

	return node;
}

NODE* SyntaxAnalyzer::_Liter()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* node{ _Integer() };
	
	// TODO: リテラル増え次第ここに追記
	/*
	* if (node != nullptr)
	* {
	*   // 文字列～
	* }
	*/

	/*if (node == nullptr)
	{
		Error("不明なリテラルです。");
	}*/

	return node;
}

NODE* SyntaxAnalyzer::_Var()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	return NewNode({ GetIdx(), NODE_REGISTER_VAR_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_CallFunc()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek(1) };

	if (token != "(")
	{
		return nullptr;  // "(" で始まらないなら関数呼び出しではない
	}

	NODE* name{ _Func() };

	Expect("(");

	NODE* args{ _Args() };

	NODE* node = NewNode({ GetIdx(), NODE_CALLFUNC, name, args });

	Expect(")");

	return node;
}

NODE* SyntaxAnalyzer::_Args()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek() };

	if (token == ")")
	{
		return nullptr;
	}

	NODE* expr{ _Expr() };
	
	NODE* next{ nullptr };
	if (Consume(","))
	{
		next = _Args();
	}

	return NewNode({ GetIdx(), NODE_ARG, expr, next });
}

NODE* SyntaxAnalyzer::_NFor()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	Expect("(");

	NODE* init{ _VarDec() };  // 
	if (init == nullptr)
	{
		init = _Assign();
	}

	Expect(";");

	NODE* expr{ _Expr() };  // 
	
	Expect(";");

	NODE* updt{ nullptr };
	
	updt = _Assign();

	if (updt == nullptr)  // 代入式ではない
	{
		updt = _Expr();  //
	}


	Expect(")");

	NODE* proc{ _Block() };  // 
	if (proc == nullptr)
	{
		proc = Procs();
	}

	return NewNode({ GetIdx(), NODE_NFOR, init, expr, updt, proc });
}

NODE* SyntaxAnalyzer::_NIf()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	Expect("(");

	NODE* expr{ _Expr() };

	Expect(")");

	NODE* proc{ _Block() };  // 
	if (proc == nullptr)
	{
		proc = Procs();
	}

	return NewNode({ GetIdx(), NODE_NIF, expr, proc });
}

NODE* SyntaxAnalyzer::_Block()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Consume("{"))
	{
		return Procs();
	}
	else  // 波かっこが始まらないならブロックではない
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::Procs(int _callCount)
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (_callCount > 1000)
	{
		Error("構文エラー");
		return nullptr;
	}

	if (Consume("}"))  // 波かっこ閉じられてたならブロック終了
	{
		return nullptr;
	}
	else
	{
		return NewNode({ GetIdx(), NODE_PROC, Proc(), Procs(++_callCount) });
	}
}

NODE* SyntaxAnalyzer::Proc()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Consume("for"))
	{
		return _NFor();
	}
	else if (Consume("if"))
	{
		return _NIf();
	}
	else
	{
		NODE* node{ _VarDec() };
		if (node != nullptr)  // 変数宣言
		{
			Expect(";");
			return node;
		}
		node = _Return();
		if (node != nullptr)  // 返却式
		{
			Expect(";");
			return node;
		}
		node = _CallFunc();
		if (node != nullptr)  // 関数呼び出し
		{
			Expect(";");
			return node;
		}
		node = _Assign();  // 代入式
		if (node != nullptr)
		{
			Expect(";");
			return node;
		}

		if (Consume(";"))
		{
			Error("不要なセミコロンがあります。");
			return nullptr;
		}

		node = _Expr();  // 式
		Expect(";");
		return node;
	}
}

NODE* SyntaxAnalyzer::_Func()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	return NewNode({ GetIdx(), NODE_REGISTER_FUNC_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_Return()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Consume("return"))
	{
		return NewNode({ GetIdx(), NODE_RET, _Expr() });
	}
	else  // return が書かれていないならreturnではない
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Params()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek() };
	if (token == ")")
	{
		return nullptr;
	}

	NODE* varDec{ _VarDec() };
	
	NODE* next{ nullptr };
	if (Consume(","))
	{
		next = _Params();
	}

	return NewNode({ GetIdx(), NODE_PARAM, varDec, next });
}

NODE* SyntaxAnalyzer::_FuncDef()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (IsUnreadable(4))
	{
		return nullptr;
	}

	NODE* type{ _Type() };

	if (type == nullptr)
	{
		Error("戻り値が不明な型");
		return nullptr;
	}

	NODE* name{ _Name() };

	if (name == nullptr)
	{
		Error("使用不可能な名前が使用されている");
		return nullptr;
	}

	Expect("(");

	NODE* params{ _Params() };

	Expect(")");

	NODE* block{ _Block() };

	/*if (block == nullptr)
	{
		Error("処理ブロックが記述されていない");
		return nullptr;
	}*/

	return NewNode({ GetIdx(), NODE_FUNCDEC, type, name, params, block });
}

NODE* SyntaxAnalyzer::_Name()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek() };

	auto itr = token.begin();

	if (!::isalpha(*itr) && *itr != '_')
	{
		Error("名前の頭はアルファベットか、アンダースコアしか使えないよ");
		return nullptr;
	}

	itr++;
	for (; itr != token.end(); itr++)
	{
		if (::isalnum(*itr) == false && *itr != '_')
		{
			Error("名前にはアルファベットか、数字、アンダースコアしか使えないよ");
			return nullptr;
		}
	}

	return NewNode({ Advance(), NODE_NAME });
}

NODE* SyntaxAnalyzer::_Type()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek() };

	if (token == "int"
		|| token == "void")
	{
		return NewNode({ Advance(), NODE_TYPE });
	}

	// TODO: 今後型を登録可能になり次第ここを改良
	// Error("不明な型");
	return nullptr;  // 型ではない
}

NODE* SyntaxAnalyzer::_Integer()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	const std::string& token{ Peek() };

	for (auto itr = token.begin(); itr != token.end(); itr++)
	{
		if (::isdigit(*itr))
		{
			continue;
		}
		return nullptr;
	}

	return NewNode({ Advance(), NODE_INTEGER });
}

NODE* SyntaxAnalyzer::_VarDec()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	NODE* type{ _Type() };
	if (type == nullptr)
	{
		return nullptr;  // 型ではないなら変数宣言ではない
	}

	NODE* assign = _Assign();

	return NewNode({ GetIdx(), NODE_VARDEC, type, assign });
}

NODE* SyntaxAnalyzer::_Assign()
{
	// エラーあるならreturn
	if (HasError()) return nullptr;

	if (Peek(1) != "=")
	{
		return nullptr;  // 代入式ではない
	}

	NODE* name{ _Name() };

	NODE* expr{ nullptr };
	
	if (Consume("="))  // 初期値あり
	{
		expr = _Expr();
	}

	NODE* next{ nullptr };

	if (Consume(","))
	{
		next = _Assign();
	}

	return NewNode({ GetIdx(), NODE_ASSIGN, name, expr, next });
}

#pragma endregion

bool SyntaxAnalyzer::IsUnreadable(const size_t _size)
{
	return readIndex_ + _size > in_.size();
}

int SyntaxAnalyzer::Advance()
{
	const int INDEX{ readIndex_ };

	readIndex_++;

	return INDEX;
}

const std::string& SyntaxAnalyzer::Peek(const int _offset)
{
	const int INDEX{ readIndex_ + _offset };
	assert(0 <= INDEX && INDEX < in_.size()
		&& "範囲外のトークンを取得しようとした @SyntaxAnalyzer::Peek");

	const std::string& TOKEN{ in_[INDEX].second };
	assert(TOKEN.size() > 0
		&& "取り出したトークンがない @SyntaxAnalyzer::Peek");

	return TOKEN;
}

NODE* SyntaxAnalyzer::NewNode(const NODE& _node)
{
	static int count{ 0 };
	count++;
	if (count > 1000)
	{
		Error("未解決のトークンです。");
		return nullptr;
	}

	NODE* pNode{ new NODE{ _node } };
	out_.push_back(pNode);
	return pNode;
}

bool SyntaxAnalyzer::Consume(const std::string& _str)
{
	if (readIndex_ >= in_.size())
	{
		Error("エラー");
		readIndex_++;
		return true;
	}

	if (in_[readIndex_].second == _str)
	{
		readIndex_++;
		return true;
	}
	return false;
}

void SyntaxAnalyzer::Expect(const std::string& _str)
{
	if (readIndex_ >= in_.size()) return;

	if (in_[readIndex_].second == _str)
	{
		readIndex_++;
	}
	else
	{
		std::string message{ _str};
		message += " が抜けているよ";
		Error(message.c_str());
	}
}

void SyntaxAnalyzer::Error(const char* _message)
{
	if (readIndex_ < 0 || in_.size() <= readIndex_)
	{
		ErrorFull(_message, { GetIdx(), -1 });
	}
	else
	{
		ErrorFull(_message, in_[readIndex_].first);
	}
	readIndex_ = in_.size();
}

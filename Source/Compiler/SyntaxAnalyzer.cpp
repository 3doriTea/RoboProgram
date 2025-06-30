#include "SyntaxAnalyzer.h"
#include <cassert>

void SyntaxAnalyzer::Analyze()
{
	for (readIndex_ = 0; readIndex_ < in_.size(); readIndex_++)
	{
	}
}

NODE* SyntaxAnalyzer::_Expr()
{
	return _Lor();
}

#pragma region スタック解決する関数

NODE* SyntaxAnalyzer::_Lor()
{
	NODE* node{ _Land() };

	while (true)
	{
		if (Consume("||"))
		{
			node = NewNode({ -1, NODE_OR, node, _Land() });
		}
		else
		{
			return node;
		}
	}
}

NODE* SyntaxAnalyzer::_Land()
{
	NODE* node{ _Equal() };

	while (true)
	{
		if (Consume("&&"))
		{
			node = NewNode({ -1, NODE_AND, node, _Equal() });
		}
		else
		{
			return node;
		}
	}
}

NODE* SyntaxAnalyzer::_Equal()
{
	NODE* node{ _Then() };

	while (true)
	{
		if (Consume("=="))
		{
			node = NewNode({ -1, NODE_EQUAL, node, _Then() });
		}
		else if (Consume("!="))
		{
			node = NewNode({ -1, NODE_NOTEQUAL, node, _Then() });
		}
		else
		{
			return node;
		}
	}
}

NODE* SyntaxAnalyzer::_Then()
{
	NODE* node{ _Add() };

	while (true)
	{
		if (Consume("<"))
			node = NewNode({ -1, NODE_LESSTHAN, node, _Add() });
		else if (Consume(">"))
			node = NewNode({ -1, NODE_GREATERTHEN, node, _Add() });
		else if (Consume("<="))
			node = NewNode({ -1, NODE_LESSEQUAL, node, _Add() });
		else if (Consume(">="))
			node = NewNode({ -1, NODE_GREATEREQUAL, node, _Add() });
		else
			return node;
	}
}

NODE* SyntaxAnalyzer::_Add()
{
	NODE* node{ _Mul() };

	while (true)
	{
		if (Consume("+"))
		{
			node = NewNode({ -1, NODE_AND, node, _Mul() });
		}
		else if (Consume("-"))
		{
			node = NewNode({ -1, NODE_SUB, node, _Mul() });
		}
	}
}

NODE* SyntaxAnalyzer::_Mul()
{
	NODE* node{ _Unary() };

	while (true)
	{
		if (Consume("*"))
		{
			node = NewNode({ -1, NODE_MUL, node, _Unary() });
		}
		else if (Consume("/"))
		{
			node = NewNode({ -1, NODE_DIV, node, _Unary() });
		}
	}
}

NODE* SyntaxAnalyzer::_Unary()
{
	if (Consume("+"))
	{
		return _Postfix();
	}
	if (Consume("-"))
	{
		return NewNode({ -1, NODE_SUB, nullptr, _Postfix() });
	}
	return _Primary();
}

NODE* SyntaxAnalyzer::_Postfix()
{
	NODE* node{ _Primary() };

	if (Consume("++"))
	{
		node = NewNode({ -1, NODE_INCREMENT, node });
	}
	else if (Consume("--"))
	{
		node = NewNode({ -1, NODE_DECREMENT, node });
	}
	else
	{
		return node;
	}
}

NODE* SyntaxAnalyzer::_Primary()
{
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
	NODE* node{ _Integer() };
	
	// TODO: リテラル増え次第ここに追記
	/*
	* if (node != nullptr)
	* {
	*   // 文字列～
	* }
	*/

	if (node == nullptr)
	{
		Error("不明なリテラルです。");
	}

	return node;
}

NODE* SyntaxAnalyzer::_Var()
{
	return NewNode({ -1, NODE_REGISTER_VAR_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_CallFunc()
{
	const std::string& token{ Peek(1) };

	if (token != "(")
	{
		return nullptr;  // "(" で始まらないなら関数呼び出しではない
	}

	NODE* node = NewNode({ -1, NODE_CALLFUNC, _Name(), _Args() });

	Expect(")");

	return node;
}

NODE* SyntaxAnalyzer::_Args()
{
	NODE* expr{ _Expr() };
	
	NODE* next{ nullptr };
	if (Consume(","))
	{
		next = _Args();
	}

	return NewNode({ -1, NODE_ARG, expr, next });
}

NODE* SyntaxAnalyzer::_NFor()
{
	Expect("(");

	NODE* init{ _VarDec() };  // 
	if (init == nullptr)
	{
		init = _Assign();
	}

	Expect(";");

	NODE* expr{ _Expr() };  // 
	
	Expect(";");
	
	NODE* updt{ _Expr() };  // 

	Expect(")");

	NODE* proc{ _Block() };  // 
	if (proc == nullptr)
	{
		proc = Procs();
	}

	return NewNode({ -1, NODE_NFOR, init, expr, updt, proc });
}

NODE* SyntaxAnalyzer::_NIf()
{
	Expect("(");

	NODE* expr{ _Expr() };

	Expect(")");

	NODE* proc{ _Block() };  // 
	if (proc == nullptr)
	{
		proc = Procs();
	}

	return NewNode({ -1, NODE_NIF, expr, proc });
}

NODE* SyntaxAnalyzer::_Block()
{
	if (Consume("{"))
	{
		return Procs();
	}
	else  // 波かっこが始まらないならブロックではない
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::Procs()
{
	if (Consume("}"))  // 波かっこ閉じられてたならブロック終了
	{
		return nullptr;
	}
	else
	{
		return NewNode({ -1, NODE_PROC, Proc(), Procs() });
	}
}

NODE* SyntaxAnalyzer::Proc()
{
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
			return node;
		}
		node = _Return();
		if (node != nullptr)  // 返却式
		{
			return node;
		}
		return _Assign();  // 代入式
	}
}

NODE* SyntaxAnalyzer::_Return()
{
	if (Consume("return"))
	{
		return NewNode({ -1, NODE_RET, _Expr() });
	}
	else  // return が書かれていないならreturnではない
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Params()
{
	NODE* varDec{ _VarDec() };
	
	NODE* next{ nullptr };
	if (Consume(","))
	{
		next = _Params();
	}

	return NewNode({ -1, NODE_PARAM, varDec, next });
}

//NODE* SyntaxAnalyzer::_Params()
//{
//	NODE* node{ NewNode({ -1, NODE_PARAMS, _Name(), nullptr }) };
//
//	NODE* param{ nullptr };
//
//	while (true)
//	{
//		if (Consume(")"))
//		{
//			break;
//		}
//		if (param == nullptr)
//		{
//			node->param.expr = _Expr();
//		}
//		else
//		{
//			param->param.next =
//		}
//	}
//
//	return nullptr;
//}

NODE* SyntaxAnalyzer::_FuncDef()
{
	NODE* type{ _Type() };
	
	assert(type != nullptr
		&& "関数定義で不明な型が記述されている @SyntaxAnalyzer::_FuncDef");

	NODE* name{ _Name() };

	assert(name != nullptr
		&& "使用不可能な変数名が記述されている @SyntaxAnalyzer::_FuncDef");

	Expect("(");

	NODE* params{ _Params() };

	Expect(")");

	NODE* block{ _Block() };

	assert(block != nullptr
		&& "処理ブロックが記述されていない @SyntaxAnalyzer::_FuncDef");

	return NewNode({ -1, NODE_FUNCDEC, type, name, params, block });
}

NODE* SyntaxAnalyzer::_Name()
{
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
	NODE* type{ _Type() };
	if (type == nullptr)
	{
		return nullptr;  // 型ではないなら変数宣言ではない
	}

	NODE* assign = _Assign();

	return NewNode({ -1, NODE_VARDEC, type, assign });
}

NODE* SyntaxAnalyzer::_Assign()
{
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

	return NewNode({ -1, NODE_ASSIGN, name, expr, next });
}

#pragma endregion

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
	out_.push_back(_node);
	return &(*out_.end());
}

bool SyntaxAnalyzer::Consume(const std::string& _str)
{
	if (in_[readIndex_].second == _str)
	{
		readIndex_++;
		return true;
	}
	return false;
}

void SyntaxAnalyzer::Expect(const std::string& _str)
{
	if (in_[readIndex_].second == _str)
	{
		readIndex_++;
	}
	else
	{
		Error("丸括弧が閉じられていない");
	}
}

void SyntaxAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, in_[readIndex_].first);
}

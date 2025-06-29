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
	const std::string& token{ Peek() };

	assert(token.size() > 0
		&& "取り出したトークンがない @SyntaxAnalyzer::_Liter");

	for (auto itr = token.begin(); itr != token.end(); itr++)
	{
		if (::isdigit(*itr))
		{
			continue;
		}
		return nullptr;
	}

	return NewNode({ Advance(), NODE_NUMBER, nullptr});
}

NODE* SyntaxAnalyzer::_Var()
{
	return nullptr;
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
	return in_[INDEX].second;
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

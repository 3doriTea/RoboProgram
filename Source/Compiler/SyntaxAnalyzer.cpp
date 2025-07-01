#include "SyntaxAnalyzer.h"
#include <cassert>

void SyntaxAnalyzer::Analyze()
{
	for (readIndex_ = 0; readIndex_ < in_.size(); readIndex_++)
	{
		_Global();
	}
}

NODE* SyntaxAnalyzer::_Global()
{
	NODE* node{ nullptr };

	for (readIndex_ = 0; readIndex_ < in_.size(); readIndex_++)
	{
		node = NewNode({ -1, NODE_GLOBAL, _FuncDef(), node });
	}

	return node;
}

NODE* SyntaxAnalyzer::_Expr()
{
	return _Lor();
}

#pragma region �X�^�b�N��������֐�

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

	//while (true)
	for( ; ; )
	{
		if (Consume("+"))
		{
			node = NewNode({ -1, NODE_ADD, node, _Mul() });
		}
		else if (Consume("-"))
		{
			node = NewNode({ -1, NODE_SUB, node, _Mul() });
		}
		else
		{
			return node;
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
		else
		{
			return node;
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

	return node;
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
	
	// TODO: ���e�����������悱���ɒǋL
	/*
	* if (node != nullptr)
	* {
	*   // ������`
	* }
	*/

	/*if (node == nullptr)
	{
		Error("�s���ȃ��e�����ł��B");
	}*/

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
		return nullptr;  // "(" �Ŏn�܂�Ȃ��Ȃ�֐��Ăяo���ł͂Ȃ�
	}

	NODE* name{ _Func() };

	Expect("(");

	NODE* args{ _Args() };

	NODE* node = NewNode({ -1, NODE_CALLFUNC, name, args });

	Expect(")");

	return node;
}

NODE* SyntaxAnalyzer::_Args()
{
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
	else  // �g���������n�܂�Ȃ��Ȃ�u���b�N�ł͂Ȃ�
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::Procs(int _callCount)
{
	if (_callCount > 1000)
	{
		Error("�\���G���[");
		return nullptr;
	}

	if (Consume("}"))  // �g�����������Ă��Ȃ�u���b�N�I��
	{
		return nullptr;
	}
	else
	{
		return NewNode({ -1, NODE_PROC, Proc(), Procs(++_callCount) });
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
		if (node != nullptr)  // �ϐ��錾
		{
			Expect(";");
			return node;
		}
		node = _Return();
		if (node != nullptr)  // �ԋp��
		{
			Expect(";");
			return node;
		}
		node = _CallFunc();
		if (node != nullptr)  // �֐��Ăяo��
		{
			Expect(";");
			return node;
		}
		node = _Assign();  // �����
		Expect(";");
		return node;
	}
}

NODE* SyntaxAnalyzer::_Func()
{
	return NewNode({ -1, NODE_REGISTER_FUNC_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_Return()
{
	if (Consume("return"))
	{
		return NewNode({ -1, NODE_RET, _Expr() });
	}
	else  // return ��������Ă��Ȃ��Ȃ�return�ł͂Ȃ�
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Params()
{
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

	return NewNode({ -1, NODE_PARAM, varDec, next });
}

NODE* SyntaxAnalyzer::_FuncDef()
{
	if (IsUnreadable(4))
	{
		return nullptr;
	}

	NODE* type{ _Type() };

	if (type == nullptr)
	{
		Error("�߂�l���s���Ȍ^");
		return nullptr;
	}

	NODE* name{ _Name() };

	if (name == nullptr)
	{
		Error("�g�p�s�\�Ȗ��O���g�p����Ă���");
		return nullptr;
	}

	Expect("(");

	NODE* params{ _Params() };

	Expect(")");

	NODE* block{ _Block() };

	if (block == nullptr)
	{
		Error("�����u���b�N���L�q����Ă��Ȃ�");
		return nullptr;
	}

	return NewNode({ -1, NODE_FUNCDEC, type, name, params, block });
}

NODE* SyntaxAnalyzer::_Name()
{
	const std::string& token{ Peek() };

	auto itr = token.begin();

	if (!::isalpha(*itr) && *itr != '_')
	{
		Error("���O�̓��̓A���t�@�x�b�g���A�A���_�[�X�R�A�����g���Ȃ���");
		return nullptr;
	}

	itr++;
	for (; itr != token.end(); itr++)
	{
		if (::isalnum(*itr) == false && *itr != '_')
		{
			Error("���O�ɂ̓A���t�@�x�b�g���A�����A�A���_�[�X�R�A�����g���Ȃ���");
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

	// TODO: ����^��o�^�\�ɂȂ莟�悱��������
	// Error("�s���Ȍ^");
	return nullptr;  // �^�ł͂Ȃ�
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
		return nullptr;  // �^�ł͂Ȃ��Ȃ�ϐ��錾�ł͂Ȃ�
	}

	NODE* assign = _Assign();

	return NewNode({ -1, NODE_VARDEC, type, assign });
}

NODE* SyntaxAnalyzer::_Assign()
{
	NODE* name{ _Name() };

	NODE* expr{ nullptr };
	
	if (Consume("="))  // �����l����
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
		&& "�͈͊O�̃g�[�N�����擾���悤�Ƃ��� @SyntaxAnalyzer::Peek");

	const std::string& TOKEN{ in_[INDEX].second };
	assert(TOKEN.size() > 0
		&& "���o�����g�[�N�����Ȃ� @SyntaxAnalyzer::Peek");

	return TOKEN;
}

NODE* SyntaxAnalyzer::NewNode(const NODE& _node)
{
	//out_.push_back(_node);
	//return &(out_.data()[out_.size() - 1]);

	NODE* pNode{ new NODE{ _node } };
	out_.push_back(pNode);
	return pNode;
}

bool SyntaxAnalyzer::Consume(const std::string& _str)
{
	if (readIndex_ >= in_.size())
	{
		Error("�G���[");
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
	if (in_[readIndex_].second == _str)
	{
		readIndex_++;
	}
	else
	{
		std::string message{ _str};
		message += " �������Ă����";
		Error(message.c_str());
	}
}

void SyntaxAnalyzer::Error(const char* _message)
{
	if (readIndex_ < 0 || in_.size() <= readIndex_)
	{
		ErrorFull(_message, { -1, -1 });
	}
	else
	{
		ErrorFull(_message, in_[readIndex_].first);
	}
}

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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	if (readIndex_ >= in_.size())
	{
		return nullptr;
	}

	return NewNode({ GetIdx(), NODE_GLOBAL, _FuncDef(), _Global() });
}

NODE* SyntaxAnalyzer::_Expr()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	return _Lor();
}

#pragma region �X�^�b�N��������֐�

NODE* SyntaxAnalyzer::_Lor()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Land()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Equal()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Then()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Add()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Mul()
{
	// �G���[����Ȃ�return
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
		// �G���[������Ȃ�return
		if (HasError()) return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Unary()
{
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	return NewNode({ GetIdx(), NODE_REGISTER_VAR_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_CallFunc()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	const std::string& token{ Peek(1) };

	if (token != "(")
	{
		return nullptr;  // "(" �Ŏn�܂�Ȃ��Ȃ�֐��Ăяo���ł͂Ȃ�
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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
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

	if (updt == nullptr)  // ������ł͂Ȃ�
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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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
		return NewNode({ GetIdx(), NODE_PROC, Proc(), Procs(++_callCount) });
	}
}

NODE* SyntaxAnalyzer::Proc()
{
	// �G���[����Ȃ�return
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
		if (node != nullptr)
		{
			Expect(";");
			return node;
		}

		if (Consume(";"))
		{
			Error("�s�v�ȃZ�~�R����������܂��B");
			return nullptr;
		}

		node = _Expr();  // ��
		Expect(";");
		return node;
	}
}

NODE* SyntaxAnalyzer::_Func()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	return NewNode({ GetIdx(), NODE_REGISTER_FUNC_NAME, _Name() });
}

NODE* SyntaxAnalyzer::_Return()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	if (Consume("return"))
	{
		return NewNode({ GetIdx(), NODE_RET, _Expr() });
	}
	else  // return ��������Ă��Ȃ��Ȃ�return�ł͂Ȃ�
	{
		return nullptr;
	}
}

NODE* SyntaxAnalyzer::_Params()
{
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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

	/*if (block == nullptr)
	{
		Error("�����u���b�N���L�q����Ă��Ȃ�");
		return nullptr;
	}*/

	return NewNode({ GetIdx(), NODE_FUNCDEC, type, name, params, block });
}

NODE* SyntaxAnalyzer::_Name()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

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
	// �G���[����Ȃ�return
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
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	NODE* type{ _Type() };
	if (type == nullptr)
	{
		return nullptr;  // �^�ł͂Ȃ��Ȃ�ϐ��錾�ł͂Ȃ�
	}

	NODE* assign = _Assign();

	return NewNode({ GetIdx(), NODE_VARDEC, type, assign });
}

NODE* SyntaxAnalyzer::_Assign()
{
	// �G���[����Ȃ�return
	if (HasError()) return nullptr;

	if (Peek(1) != "=")
	{
		return nullptr;  // ������ł͂Ȃ�
	}

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
		&& "�͈͊O�̃g�[�N�����擾���悤�Ƃ��� @SyntaxAnalyzer::Peek");

	const std::string& TOKEN{ in_[INDEX].second };
	assert(TOKEN.size() > 0
		&& "���o�����g�[�N�����Ȃ� @SyntaxAnalyzer::Peek");

	return TOKEN;
}

NODE* SyntaxAnalyzer::NewNode(const NODE& _node)
{
	static int count{ 0 };
	count++;
	if (count > 1000)
	{
		Error("�������̃g�[�N���ł��B");
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
	if (readIndex_ >= in_.size()) return;

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
		ErrorFull(_message, { GetIdx(), -1 });
	}
	else
	{
		ErrorFull(_message, in_[readIndex_].first);
	}
	readIndex_ = in_.size();
}

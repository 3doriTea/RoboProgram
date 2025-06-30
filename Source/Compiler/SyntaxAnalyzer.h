#pragma once
#include "Analyzer.h"
#include <map>
#include <stack>


///// <summary>
///// �^���
///// </summary>
//struct TypeInfo
//{
//	Byte size;  // �^�̃T�C�Y
//	const TokenString& name;  // �^��
//};

/// <summary>
/// �\����͂���N���X
/// </summary>
class SyntaxAnalyzer : public Analyzer<
	SyntaxAnalyzer,
	Tokens,
	Nodes,
	SOURCE_POS>
{
public:
	using Analyzer::Analyzer;
	/// <summary>
	/// ��͂���
	/// </summary>
	void Analyze() override;

private:
#pragma region �X�^�b�N��������֐�
	NODE* _Global();

	NODE* _Expr();
	NODE* _Lor();
	NODE* _Land();
	NODE* _Equal();
	NODE* _Then();
	NODE* _Add();
	NODE* _Mul();
	NODE* _Unary();
	NODE* _Postfix();
	NODE* _Primary();

	NODE* _Val();
	NODE* _Liter();
	NODE* _Var();
	NODE* _CallFunc();

	NODE* _Args();
	
	NODE* _NFor();
	NODE* _NIf();
	NODE* _Block();

	NODE* Procs(int _callCount = 0);
	NODE* Proc();

	NODE* _Func();

	NODE* _Return();

	NODE* _Params();

	NODE* _FuncDef();

	NODE* _Name();
	NODE* _Type();
	NODE* _Integer();

	NODE* _VarDec();
	NODE* _Assign();
#pragma endregion


#pragma region ����Ƃ�

	/// <summary>
	/// ���ɐi�߂�
	/// </summary>
	/// <returns>���݂̃C���f�b�N�X</returns>
	int Advance();

	/// <summary>
	/// �`��������
	/// </summary>
	/// <param name="_offset">�ǂݎ��ʒu����̃I�t�Z�b�g</param>
	/// <returns>�g�[�N����readonly</returns>
	const std::string& Peek(const int _offset = 0);

	NODE* NewNode(const NODE& _node);
	/// <summary>
	/// ��v���Ă�����i�߂�
	/// </summary>
	/// <param name="_str">��r������</param>
	/// <returns>��v���Ă��� true / false</returns>
	bool Consume(const std::string& _str);
	/// <summary>
	/// ����Ď̂Ă�
	/// </summary>
	/// <param name="_str"></param>
	void Expect(const std::string& _str);
#pragma endregion

	SyntaxAnalyzer& Get() { return *this; }
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const char* _message) override;

private:
	int readIndex_{};
	std::map<TokenString, Byte> typeRegister_{};
	std::map<TokenString, Byte> funcRegister_{};
	std::stack<std::map<TokenString, Byte>> varRegister_{};
};

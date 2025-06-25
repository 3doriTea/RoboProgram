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
	NODE* Global();
	NODE* FuncDef();
#pragma endregion


#pragma region ����Ƃ�
	bool Consume(const std::string& _str);
	bool Except(const std::string& _str);
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

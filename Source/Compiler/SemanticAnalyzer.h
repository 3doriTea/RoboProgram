#pragma once
#include "Analyzer.h"
#include <map>
#include <string>

class SemanticAnalyzer : public Analyzer<
	SemanticAnalyzer,
	std::pair<Nodes&, Tokens&>,
	ByteCodes,
	SOURCE_POS>
{
private:
	struct FuncData
	{
		FuncData() :
			index{ -1 },
			byteCodes{}
		{}

		int retTypeSize;
		int index;
		ByteCodes byteCodes;
	};

	struct UseMemory
	{
		UseMemory() :
			UseMemory{ -1, 0 }
		{}
		UseMemory(const int _offset, const int _size) :
			offset{ _offset },
			size{ _size }
		{}

		int offset;        // �d�l�������ʒu
		int size;          // �g�p�������T�C�Y
	};

	struct TypeInfo
	{
		enum Type
		{
			INT,
			BOOL,
		};
	};
public:
	using Analyzer::Analyzer;
	/// <summary>
	/// ��͂���
	/// </summary>
	void Analyze() override;

private:
	/// <summary>
	/// �������̈���m�ۂ���
	/// </summary>
	/// <param name="name">��Ԃ̖��O</param>
	/// <param name="size">�v���T�C�Y</param>
	/// <returns>�m�ۂł����������̔Ԓn</returns>
	int NewMemory(const std::string name, int size);

	/// <summary>
	/// �t�������O���烁�����̗̈���擾����
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	int GetMemory(const std::string& name);

	void RegSet(const int value, ByteCodes& bc, int offset = 0);
	void RegSet(const Byte value, ByteCodes& bc, int offset = 0);

	void Read(const NODE* n, const int _depth, int position = 0);
	void ReadFuncDec(const NODE* n, ByteCodes& bc, int position);
	std::string ReadName(const NODE* n);

	std::string ReadFuncName(const NODE* n, ByteCodes& bc);

	/// <summary>
	/// �^���擾 = �^�̃T�C�Y ���擾
	/// </summary>
	/// <param name="n"></param>
	/// <returns>�^�� byte �T�C�Y</returns>
	int ReadType(const NODE* n);
	int ReadInteger(const NODE* n);
	void ReadParam(const NODE* n, ByteCodes& bc);
	void ReadProcs(const NODE* n, ByteCodes& bc, int begin);

	void ReadGlobal(const NODE* n, int begin);

	void ReadRet(const NODE* n, ByteCodes& bc);

	void ReadArg(const NODE* n, ByteCodes& bc);

	void ReadNFor(const NODE* n, ByteCodes& bc, int position);

	void ReadNIf(const NODE* n, ByteCodes& bc, int position);

	void ReadExpr(const NODE* n, ByteCodes& bc);

	void ReadAssign(const NODE* n, ByteCodes& bc, const int typeSize);

	/// <summary>
	/// �ϐ��̎Q�Ƃ��A�X�^�b�N�ɒǉ�
	/// </summary>
	/// <param name="n"></param>
	/// <param name="bc"></param>
	void ReadVar(const NODE* n, ByteCodes& bc);

	/// <summary>
	/// �ϐ���錾���A�������m�ۂ���
	/// </summary>
	/// <param name="n"></param>
	/// <param name="bc"></param>
	/// <param name="allowInit">����������\ true / false</param>
	/// <param name="consecutive ">�A���錾�\�� true / false</param>
	void ReadVarDec(const NODE* n, ByteCodes& bc, bool allowInit = false, bool consecutive = false);

	/// <summary>
	/// �֐����Q�Ƃ��A���s����R�[�h��ǋL
	/// </summary>
	/// <param name="n"></param>
	/// <param name="bc"></param>
	void ReadCallFunc(const NODE* n, ByteCodes& bc);

	/// <summary>
	/// ���W�X�^���烁�����ɒl���Z�b�g����
	/// </summary>
	/// <param name="_addr"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void MemorySet(const int _addr, const int _size, ByteCodes& bc);

	/// <summary>
	/// ���������烌�W�X�^�ɒl���Z�b�g����
	/// </summary>
	/// <param name="_addr"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void MemoryGet(const int _addr, const int _size, ByteCodes& bc);

	/// <summary>
	/// ���W�X�^����X�^�b�N�ɒl���v�b�V������
	/// </summary>
	/// <param name="_regOffset"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void StackPush(const int _regOffset, const int _size, ByteCodes& bc);
	/// <summary>
	/// �X�^�b�N�Ƀ��W�X�^����l���|�b�v����
	/// </summary>
	/// <param name="_regOffset"></param>
	/// <param name="_size"></param>
	/// <param name="bc"></param>
	void StackPop(const int _regOffset, const int _size, ByteCodes& bc);

	SemanticAnalyzer& Get() { return *this; }
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const char* _message) override;
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const std::string& _message);


	int maxMemorySize{};
	std::map<std::string, UseMemory> memory{};
	std::map<std::string, FuncData> funcGroup{};
};

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

	void Read(const NODE* n, const int _depth);
	void ReadFuncDec(const NODE* n);
	std::string ReadName(const NODE* n);
	std::string ReadType(const NODE* n);
	int ReadIntegier(const NODE* n);
	void ReadParam(const NODE* n, ByteCodes& bc);
	void ReadProcs(const NODE* n, ByteCodes& bc);

	void ReadExpr(const NODE* n, ByteCodes& bc);

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


	SemanticAnalyzer& Get() { return *this; }
	/// <summary>
	/// �G���[����
	/// </summary>
	/// <param name="_message">�G���[���b�Z�[�W</param>
	void Error(const char* _message) override;


	int maxMemorySize{};
	std::map<std::string, UseMemory> memory{};
	std::map<std::string, FuncData> funcGroup{};
};

#include "Compiler.h"

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"

#include "../IO/FileSaver.h"


Compiler::Compiler(
	const std::vector<std::string>& _srcLines,
	ByteCodes& _byteCodes) :
	srcLines_{ _srcLines },
	byteCodes_{ _byteCodes }
{
}

Compiler::~Compiler()
{
}

void Compiler::Start()
{
	SourceLines sourceLines{};
	
	for (int l = 0; l < srcLines_.size(); l++)
	{
		sourceLines.push_back({ l, srcLines_[l] });
	}

	bool isError{ false };

	Tokens tokens{};

	// �����͂�����
	LexicalAnalyzer{ sourceLines, tokens }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "�g�[�N���G���[ " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "�s ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "������ ";
				}

				errorMessage += _msg;
				onError_(errorMessage, _srcPos, ERR_LEXICAL);
				isError = true;
			})
		.Analyze();

	if (isError) return;  // �G���[�Ȃ�~�߂�

	Nodes nodes{};

	// �\�����
	SyntaxAnalyzer{ tokens, nodes }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "�\���G���[ " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "�s ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "������ ";
				}
				errorMessage += _msg;
				onError_(errorMessage, _srcPos, ERR_SYNTAX);
				isError = true;
			})
		.Analyze();

	if (isError) return;  // �G���[�Ȃ�~�߂�

	std::pair<Nodes&, Tokens&> nodeAndTokens{ nodes, tokens };

	// �Ӗ����
	SemanticAnalyzer{ nodeAndTokens, byteCodes_ }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "���@�G���[ " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "�s ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "������ ";
				}
				errorMessage += _msg;

				for (auto& node : nodeAndTokens.first)
				{
					delete node;
				}

				onError_(errorMessage, _srcPos, ERR_SEMANTIC);
				isError = true;
			})
		.Analyze();

	for (auto& node : nodeAndTokens.first)
	{
		delete node;
	}

	if (isError) return;  // �G���[�Ȃ�~�߂� ���Ǌ֌W�Ȃ�
}

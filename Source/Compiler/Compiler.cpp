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

	// 字句解析をする
	LexicalAnalyzer{ sourceLines, tokens }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "トークンエラー " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "行 ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "文字目 ";
				}

				errorMessage += _msg;
				onError_(errorMessage, _srcPos, ERR_LEXICAL);
				isError = true;
			})
		.Analyze();

	if (isError) return;  // エラーなら止める

	Nodes nodes{};

	// 構文解析
	SyntaxAnalyzer{ tokens, nodes }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "構文エラー " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "行 ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "文字目 ";
				}
				errorMessage += _msg;
				onError_(errorMessage, _srcPos, ERR_SYNTAX);
				isError = true;
			})
		.Analyze();

	if (isError) return;  // エラーなら止める

	std::pair<Nodes&, Tokens&> nodeAndTokens{ nodes, tokens };

	// 意味解析
	SemanticAnalyzer{ nodeAndTokens, byteCodes_ }
		.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
			{
				std::string errorMessage = std::string{ "文法エラー " };
				if (_srcPos.line >= 0)
				{
					errorMessage += std::to_string(_srcPos.line) + "行 ";
				}
				if (_srcPos.column >= 0)
				{
					errorMessage += std::to_string(_srcPos.column) + "文字目 ";
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

	if (isError) return;  // エラーなら止める けど関係ない
}

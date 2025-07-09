#include "Compiler.h"

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "SemanticAnalyzer.h"

#include "../IO/FileSaver.h"

namespace
{
	static const char SOURCE_FILE[]{ "SourceCode.txt" };
	static const char ERROR_FILE[]{ "ErrorLog.txt" };

	enum ErrorExitCode
	{
		ERR_LEXICAL = -3101,  // 字句エラー
		ERR_SYNTAX = -3102,  // 構文エラー
		ERR_SEMANTIC = -3103,  // 文法エラー
	};
}

Compiler::Compiler()
{
}

Compiler::~Compiler()
{
}

void Compiler::Start()
{
	SourceLines sourceLines{};
	std::vector<std::string> srcLines{ FileSaver::QuickReadTextLines(SOURCE_FILE) };

	for (int l = 0; l < srcLines.size(); l++)
	{
		sourceLines.push_back({ l, srcLines[l] });
	}

	Tokens tokens{};
	auto Error = [&, this](const std::string& _message, const SOURCE_POS _position, const ErrorExitCode _code)
		{
			FileSaver::QuickWriteText(ERROR_FILE, _message);
			exit(_code);
		};

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
				Error(errorMessage, _srcPos, ERR_LEXICAL);
			})
		.Analyze();

	Nodes nodes{};

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
				Error(errorMessage, _srcPos, ERR_SYNTAX);
			})
		.Analyze();

	std::pair<Nodes&, Tokens&> nodeAndTokens{ nodes, tokens };

	ByteCodes byteCodes{};

	SemanticAnalyzer{ nodeAndTokens, byteCodes }
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

				Error(errorMessage, _srcPos, ERR_SEMANTIC);
			})
		.Analyze();

	for (auto& node : nodeAndTokens.first)
	{
		delete node;
	}
}

#pragma once
#include "Analyzer.h"

class Compiler
{
public:
	enum ErrorExitCode
	{
		ERR_LEXICAL = -3101,  // 字句エラー
		ERR_SYNTAX = -3102,  // 構文エラー
		ERR_SEMANTIC = -3103,  // 文法エラー
	};
	using OnErrorFunc = void(const std::string& _message, const SOURCE_POS _position, const ErrorExitCode _code);
public:
	Compiler(
		const std::vector<std::string>& _srcLines,
		ByteCodes& _byteCodes);
	~Compiler();

	void Start();

	Compiler& OnError(const std::function<OnErrorFunc>& _callback) { onError_ = _callback; return *this; }

private:
	const std::vector<std::string>& srcLines_;
	ByteCodes& byteCodes_;
	std::function<OnErrorFunc> onError_;
};

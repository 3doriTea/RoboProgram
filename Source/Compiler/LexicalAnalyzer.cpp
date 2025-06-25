#include "LexicalAnalyzer.h"


void LexicalAnalyzer::Analyze()
{
	out_.clear();

	for (const auto& line : in_)
	{
		lineCount_ = line.first;

		std::string token{};
		bool isInString{ false };  // ������̒���

		for (columnCount_ = 0; columnCount_ < line.second.size(); columnCount_++)
		{
			char chr{ line.second[columnCount_] };

			if (isInString)
			{
				if (chr == '\\')
				{
					PopToken(token);
					token.push_back(chr);
					if (columnCount_ >= line.second.size() - 2)
					{
						Error("�G�X�P�[�v(\\)������Ƀ_�u���N�I�[�e�[�V������������I");
						return;
					}
					token.push_back(line.second[++columnCount_]);
					PopToken(token);
				}
				/*else if (chr == '{' || chr == '}')
				{
					PopToken(token);
					PopToken(chr);
				}*/  // �t�H�[�}�b�g�` (����Ȃ�
				else if (chr == '"')
				{
					PopToken(token);
					PopToken(chr);
					isInString = false;
				}
				else
				{
					token.push_back(chr);
				}

				continue;
			}

			if (chr == '"')
			{
				isInString = true;
				PopToken(token);
				PopToken(chr);
				continue;
			}

			#pragma region �R�����g����
			if (chr == '/')
			{
				if (columnCount_ < line.second.size() - 1)
				{
					if (line.second[columnCount_ + 1] == '/')
					{
						PopToken(token);
						break;  // �������̓��s�̓R�����g
					}
				}
			}
			#pragma endregion

			#pragma region �A���L��
			if (chr == ':'
				|| chr == '+'
				|| chr == '-'
				|| chr == '<'
				|| chr == '>'
				|| chr == '='
				|| chr == '&'
				|| chr == '|'
				|| chr == '<')
			{
				if (columnCount_ < line.second.size() - 1)
				{
					if (line.second[columnCount_ + 1] == chr)
					{
						token.push_back(chr);
						token.push_back(chr);
						columnCount_++;
						PopToken(token);
						continue;
					}
				}
			}
			#pragma endregion

			switch (chr)
			{
			case '(':
			case ')':
			case '{':
			case '}':

			case '&':
			//case ':':
			case ',':
			case ';':
			case ':':
			case '?':

			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
				PopToken(token);
				PopToken(chr);
				break;
			case '\n':
			case '\t':
			case ' ':
				PopToken(token);
				break;
			default:
				token.push_back(chr);
				break;
			}

			if (columnCount_ == line.second.size() - 1)
			{
				PopToken(token);
			}
		}

		if (isInString)
		{
			Error("������̃_�u���N�I�[�e�[�V�����������Ă��Ȃ��I");
			return;
		}
	}
}

void LexicalAnalyzer::Error(const char* _message)
{
	ErrorFull(_message, SOURCE_POS{ lineCount_, columnCount_});
}

void LexicalAnalyzer::PopToken(std::string& _token)
{
	if (_token.size() > 0)
	{
		out_.push_back({ { lineCount_, columnCount_ }, _token });
	}
	_token.clear();
}

void LexicalAnalyzer::PopToken(const char _token)
{
	std::string tokenStr{ _token };
	PopToken(tokenStr);
}

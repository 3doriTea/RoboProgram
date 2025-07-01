#include <cassert>
#include "PlayScene.h"
#include "IO/Input.h"
#include "Utility/Timer.h"
#include "Utility/StringUtility.h"
#include "IO/SourceObserver.h"

#include "Background.h"
#include "Stage.h"
#include "CodeBox.h"
#include "Player.h"
#include "ViewerBox.h"

#include "Compiler/ProtoAnalyzer.h"
#include "Compiler/LexicalAnalyzer.h"
#include "Compiler/SyntaxAnalyzer.h"
#include "Compiler/SemanticAnalyzer.h"

#include "ByteCodeDefine.h"

namespace
{
	static const char SRC_FILE_NAME[]{ "SourceCode.txt" };
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	Timer::Instance().Clear();  // �ŏ��Ƀ^�C�}�[���N���A����

	new Background{};
	Stage* pStage{ new Stage{} };
	CodeBox* pCodeBox{ new CodeBox{} };

	ViewerBox* pViewerBox{ new ViewerBox{} };
	//pViewerBox->SetPosition();

	pPlayer_ = pStage->GetPlayer();
	assert(pPlayer_ != nullptr
		&& "�X�e�[�W����v���C���[�̎擾�Ɏ��s @PlayScene::PlayScene");

	using StringUtility::ToString;

	pSourceObserver_
		->OnCreateSource([&, this](
			std::vector<std::string>& _newSource)
			{
				//printfDx("�V�K�Ń\�[�X�����ꂽ\n");
				_newSource =
				{
					u8"// ���r�[�g�Ă΂���I",
					u8"void Update()",
					u8"{",
					u8"  Run();  // ����������Ƒ����",
					u8"}",
				};
			})
		.OnUpdateSource([&, this, pCodeBox, pViewerBox](
			const std::vector<std::string>& _newSource)
			{
				bool isError{ false };
				std::string errorMessage{};
				SOURCE_POS errorPosition{};
				//printfDx("�\�[�X�t�@�C���ɕύX��������\n");
 				//pCodeBox->SetSourceLines(_newSource);
				pViewerBox->SetTextLines(_newSource);

				pPlayer_->SetByteCode(
					{
						{ -1, BCD_CALL },
						{ -1, 1 },
						{ -1, BCD_HALT },
						{ 3, BCD_ACT },
						{ 3, BCD_ACT_RUN },
						{ 4, BCD_RET }
					});

				/*std::vector<std::pair<int, Byte>> byteCodeAndLines{};
				ProtoAnalyzer* analyzer{ new ProtoAnalyzer{ _newSource, byteCodeAndLines } };
				analyzer->OnError([&, this](const char* _msg, const LineCount& _line)
					{
						
					});
				analyzer->Analyze()*/;
				
				//pPlayer_->SetByteCode(byteCodeAndLines);

				SourceLines sourceLines{};

				for (int l = 0; l < _newSource.size(); l++)
				{
					sourceLines.push_back({ l, _newSource[l] });
				}

				Tokens tokens{};

				// �����͂�����
				LexicalAnalyzer{ sourceLines, tokens }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "�g�[�N���G���[ " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "�s ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "������ ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							// printfDx("�g�[�N���G���[(%d�s:%d������) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // �G���[�Ȃ�~�߂�
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				Nodes nodes{};

				SyntaxAnalyzer{ tokens, nodes }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "�\���G���[ " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "�s ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "������ ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							//printfDx("�\���G���[(%d�s:%d������) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // �G���[�Ȃ�~�߂�
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}

				std::pair<Nodes&, Tokens&> nodeAndTokens{ nodes, tokens };

				ByteCodes byteCodes{};

				SemanticAnalyzer{ nodeAndTokens, byteCodes }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
						{
							errorMessage = std::string{ "���@�G���[ " };
							if (_srcPos.line >= 0)
							{
								errorMessage += std::to_string(_srcPos.line) + "�s ";
							}
							if (_srcPos.column >= 0)
							{
								errorMessage += std::to_string(_srcPos.column) + "������ ";
							}
							errorMessage += _msg;
							errorPosition = _srcPos;
							//printfDx("���@�G���[(%d�s:%d������) %s\n", _srcPos.line, _srcPos.column, _msg);
							isError = true;
						})
					.Analyze();

				if (isError)  // �G���[�Ȃ�~�߂�
				{
					pPlayer_->SetError(errorMessage, errorPosition);
					return;
				}
			});

	Timer::AddInterval(1.0f, [&, this]()
	{
		pSourceObserver_->Update();
	}, true);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	Input::Instance().Update();
	Timer::Instance().Update();

	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TITLE");
	}

	if (Input::IsKeyDown(KEY_INPUT_F))
	{
		OpenSrcFile();
	}
}

void PlayScene::Draw()
{
	/*DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));*/
}

void PlayScene::OpenSrcFile()
{
	ShellExecute(NULL, "open", SRC_FILE_NAME, "", "", SW_SHOW);
}

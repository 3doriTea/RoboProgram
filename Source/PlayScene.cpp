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

#include "Compiler/ProtoAnalyzer.h"
#include "Compiler/LexicalAnalyzer.h"

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
		.OnUpdateSource([&, this, pCodeBox](
			const std::vector<std::string>& _newSource)
			{
				//printfDx("�\�[�X�t�@�C���ɕύX��������\n");
 				pCodeBox->SetSourceLines(_newSource);

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

				clsDx();

				// �����͂�����
				LexicalAnalyzer{ sourceLines, tokens }
					.OnError([&, this](const char* _msg, const SOURCE_POS& _srcPos)
					{
						printfDx("(%d�s:%d������) %s\n", _srcPos.line, _srcPos.column, _msg);
					})
					.Analyze();

				for (auto&& token : tokens)
				{
					printfDx("%s, ", token.second.c_str());
				}

				// �\����͂�����
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

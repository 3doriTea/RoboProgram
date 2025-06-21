#include <cassert>
#include "PlayScene.h"
#include "IO/Input.h"
#include "Utility/Timer.h"
#include "IO/SourceObserver.h"

#include "Background.h"
#include "Stage.h"
#include "CodeBox.h"
#include "Player.h"

#include "Compiler/ProtoAnalyzer.h"

namespace
{
	static const char SRC_FILE_NAME[]{ "Test.txt" };
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	Timer::Instance().Clear();  // 最初にタイマーをクリアする

	new Background{};
	Stage* pStage{ new Stage{} };
	CodeBox* pCodeBox{ new CodeBox{} };

	pPlayer_ = pStage->GetPlayer();
	assert(pPlayer_ != nullptr
		&& "ステージからプレイヤーの取得に失敗 @PlayScene::PlayScene");

	pSourceObserver_->OnUpdateSource([&, this, pCodeBox](
		const std::vector<std::string>& _newSource)
	{
		printfDx("ソースファイルに変更があった\n");
 		pCodeBox->SetSourceLines(_newSource);

		ProtoAnalyzer* analyzer{ new ProtoAnalyzer{ _newSource } };
		std::vector<std::pair<int, Byte>> byteCodeAndLines{};
		analyzer->Analyze(byteCodeAndLines);
		pPlayer_->SetByteCode(byteCodeAndLines);
	});

	Timer::AddInterval(1.0f, [&, this]()
	{
		pSourceObserver_->Update();
	});
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
		ShellExecute(NULL, "open", "Test.txt", "", "", SW_SHOW);
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}

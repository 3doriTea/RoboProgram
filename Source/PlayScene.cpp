#include "PlayScene.h"
#include "IO/Input.h"
#include "Utility/Timer.h"
#include "IO/SourceObserver.h"


namespace
{
	static const char SRC_FILE_NAME[]{ "Test.txt" };
}

PlayScene::PlayScene() :
	pSourceObserver_{ new SourceObserver{ SRC_FILE_NAME } }
{
	pSourceObserver_->OnUpdateSource([&, this]()
	{
		printfDx("ソースファイルに変更があった\n");
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

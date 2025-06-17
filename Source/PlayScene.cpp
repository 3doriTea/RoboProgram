#include "PlayScene.h"
#include "IO/Input.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	Input::Instance()->Update();

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

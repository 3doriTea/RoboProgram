#include "TitleScene.h"
#include <cassert>
#include "StartButton.h"
#include "Background.h"
#include "Screen.h"
#include "IO/FileSaver.h"


namespace
{
	static const char BACKGROUND_IMAGE_FILE[]{ "Data/Image/titleBackground.png" };
	static const char START_BUTTON[]{ "Data/Image/StartButton.png" };
	static const int START_BUTTON_POS_Y{ 465 };
	static const char ERROR_LOG[]{ "ErrorLog.txt" };
}

TitleScene::TitleScene()
{
	new Background{ BACKGROUND_IMAGE_FILE };

	StartButton* pStartButton{ new StartButton{ START_BUTTON } };
	Rectan buttonRect{ pStartButton->GetRectWorld() };

	pStartButton->SetPosition({ static_cast<int>(Screen::WIDTH - buttonRect.width) / 2, START_BUTTON_POS_Y });
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (FileSaver::ExistFile(ERROR_LOG))
	{
		SceneManager::ChangeScene("PLAY");
	}

	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TUTORIAL");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	extern const char* Version();
}

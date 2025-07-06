#include "ClearScene.h"
#include "Background.h"
#include "IO/Input.h"

#include "ReadAssemblyButton.h"
#include "ReadCodeButton.h"
#include "FinalTestButton.h"

namespace
{
	static const char BACKGROUND_IMAGE_FILE[]{ "Data/Image/ClearBackgroundText.png" };
}


ClearScene::ClearScene()
{
	new Background{ BACKGROUND_IMAGE_FILE };

	new ReadAssemblyButton{};
	new ReadCodeButton{};
	new FinalTestButton{};
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	Input::Instance().Update();
}

void ClearScene::Draw()
{
}

void ClearScene::OpenFile(const char* _name)
{
	ShellExecute(NULL, "open", _name, "", "", SW_SHOW);
}

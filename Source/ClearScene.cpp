#include "ClearScene.h"
#include "Background.h"

#include "ReadAssemblyButton.h"

namespace
{
	static const char BACKGROUND_IMAGE_FILE[]{ "Data/Image/ClearBackgroundText.png" };
}


ClearScene::ClearScene()
{
	new Background{ BACKGROUND_IMAGE_FILE };

	new ReadAssemblyButton{};
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
}

void ClearScene::Draw()
{
}

void ClearScene::OpenFile(const char* _name)
{
	ShellExecute(NULL, "open", _name, "", "", SW_SHOW);
}

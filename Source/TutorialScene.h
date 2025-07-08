#pragma once
#include "../Library/SceneBase.h"

class TutorialScene : public SceneBase
{
public:
	TutorialScene();
	~TutorialScene();

	void Update() override;
	void Draw() override;
};

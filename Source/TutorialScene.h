#pragma once
#include "../Library/SceneBase.h"


/// <summary>
/// チュートリアルシーン
/// </summary>
class TutorialScene : public SceneBase
{
public:
	TutorialScene();
	~TutorialScene();

	void Update() override;
	void Draw() override;
};

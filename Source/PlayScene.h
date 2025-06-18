#pragma once
#include "../Library/SceneBase.h"


class SourceObserver;

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

private:
	SourceObserver* pSourceObserver_;
};

#pragma once
#include "../Library/SceneBase.h"


class SourceObserver;
class Player;

/// <summary>
/// �Q�[���v���C�̃V�[���𐧌䂷��
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
	Player* pPlayer_;
};

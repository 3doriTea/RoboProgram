#pragma once
#include "../Library/SceneBase.h"
#include <string>


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

	void OpenSrcFile();
	void OpenDocument();

	static const std::string GetAssembleText() { return assembleText_; }

private:
	SourceObserver* pSourceObserver_;
	Player* pPlayer_;
	static std::string assembleText_;
};

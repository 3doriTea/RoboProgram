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
	static const std::string GetSrcCodeText() { return srcCodeCache_; }

	static const bool GetIsFinalMode() { return isFinalMode_; }
	static const void SetIsFinalMode(const bool _isFinalMode) { isFinalMode_ = _isFinalMode; }

	static const char* GetErrorFileName();

private:
	SourceObserver* pSourceObserver_;
	Player* pPlayer_;
	static std::string srcCodeCache_;
	static std::string assembleText_;
	static bool isFinalMode_;  // �ŏI���[�h
};

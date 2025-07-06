#include "FinalTestButton.h"
#include "ClearScene.h"
#include <cassert>
#include "Screen.h"
#include "IO/FileSaver.h"
#include "PlayScene.h"
#include "Fader.h"


namespace
{
	const static char BUTTON_IMAGE[]{ "Data/Image/FinalTestButton.png" };
	const static char PLAY_SCENE_BACKGROUND_IMAGE[]{ "Data/Inage/background.png" };
}

FinalTestButton::FinalTestButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "ボタン画像読み込みに失敗");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = 335;//Screen::WIDTH / 2 - rect_.width / 2;
	rect_.y = 410;//Screen::HEIGHT - ;

	SetHintText("セーブポイントなしでロボットを動かします");
}

FinalTestButton::~FinalTestButton()
{
}

void FinalTestButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	DrawGraph(rect_.x, rect_.y, hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void FinalTestButton::OnPush()
{
	PlayScene::SetIsFinalMode(true);
	SceneManager::ChangeScene("PLAY");
}

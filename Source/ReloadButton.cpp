#include "ReloadButton.h"
#include "../Library/SceneManager.h"
#include <cassert>
#include "Screen.h"
#include "Fader.h"
#include "Player.h"


namespace
{
	static char BUTTON_IMAGE[]{ "Data/Image/RestartButton.png" };
	static const char FADE_IMAGE_FILE[]{ "Data/Image/background.png" };
	static const float FADE_TIME{ 1.0f };
}

ReloadButton::ReloadButton()
{
	hImage_ = LoadGraph(BUTTON_IMAGE);
	assert(hImage_ != 0 && "çƒì«Ç›çûÇ›É{É^ÉìâÊëúì«Ç›çûÇ›Ç…é∏îs");

	GetGraphSizeF(hImage_, &rect_.width, &rect_.height);

	rect_.x = Screen::WIDTH - ((rect_.width + BUTTON_PADDING) * 3);
	rect_.y = BUTTON_PADDING;

	SetHintText("é¿å±Ççƒì«Ç›çûÇ›Ç∑ÇÈ");
}

ReloadButton::~ReloadButton()
{
}

void ReloadButton::OnDraw(const bool _onTouching, const bool _isPushing)
{
	if (_onTouching)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	DrawGraph(rect_.x, rect_.y, hImage_, TRUE);

	if (_isPushing)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 100);
		DrawGraph(rect_.x, rect_.y, hImage_, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ReloadButton::OnPush()
{
	Player* pPlayer{ FindGameObject<Player>() };
	if (pPlayer != nullptr)
	{
		pPlayer->ShockDown();
	}
	(new Fader{ FADE_IMAGE_FILE, FADE_TIME, false })->OnFinish([&, this]()
		{
			SceneManager::ReloadScene();
		});
}

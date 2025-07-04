#include "Lightning.h"
#include <cassert>
#include "../Library/Time.h"
#include "Fader.h"


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Lightning.png" };
	static const int IMAGE_SIZE{ 80 };
	static const float TO_CURR_SCENE_TIME{ 3.0f };
	static const char FADE_IMAGE_FILE[]{ "Data/Image/background.png" };
}

Lightning::Lightning(const Vector2& _position) :
	StageObject{ _position, IMAGE_FILE },
	isShocking_{ false },
	toCurrSceneTimeLeft_{ TO_CURR_SCENE_TIME }
{
	//rect_.pivot = _position;
	//rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	SetDrawOrder(-1);

	/*hImage_ = LoadGraph(IMAGE_FILE);
	assert(hImage_ != 0
		&& "画像読み込みに失敗 @Lightning::Lightning");*/
}

Lightning::~Lightning()
{
}

void Lightning::Update()
{
	StageObject::Update();

	if (isShocking_ == false)
	{
		return;
	}

	toCurrSceneTimeLeft_ -= Time::DeltaTime();
	if (toCurrSceneTimeLeft_ <= 0.0f)
	{
		(new Fader{ FADE_IMAGE_FILE, 1, false })->OnFinish([&, this]()
			{
				SceneManager::ReloadScene();
			});
	}
}

void Lightning::Draw()
{
	StageObject::Draw();

	if (isShocking_ == false)
	{
		return;
	}

	Rectan pRect{ GetPlayer()->GetRect() };
	int effectCount{ GetRand(30) };

	Vector2 from{ GetRect().GetCenter()};
	unsigned int drawColor{};

	for (int i = 0; i < effectCount; i++)
	{
		drawColor = GetRand(5) == 0 ? 0x000000 : 0xffff00;
		Vector2 to{ pRect.x + GetRand(pRect.width), pRect.y + GetRand(pRect.height) };
		DrawLine(from.x, from.y, to.x, to.y, drawColor, 2);
		from = to;
		if (GetRand(3) == 0)
		{
			from = GetRect().GetCenter();
		}
	}
}

void Lightning::OnPlayer()
{
	isShocking_ = true;
	GetPlayer()->ShockDown();  // プレイヤにshockダメージ
}

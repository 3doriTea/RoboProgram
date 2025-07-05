#include "BlockEffect.h"
#include "../Library/Time.h"
#include "Stage.h"
#include "Utility/Easings.h"


namespace
{
	static const int TILE_WIDTH{ 80 };
	static const int TILE_HEIGHT{ 80 };
	static const float EFFECT_TIME{ 1.0f };
	static const int FRAME_WIDTH{ 10 };  // エフェクトの枠
}

BlockEffect::BlockEffect(const Vector2Int& _tilePosition, unsigned int _color) :
	Object2D{},
	timeLeft_{ EFFECT_TIME },
	color_{ _color }
{
	rect_.pivot = pStage_->ToWorldPosition(_tilePosition);
	rect_.size = { TILE_WIDTH, TILE_HEIGHT };
}

BlockEffect::~BlockEffect()
{
}

void BlockEffect::Update()
{
	timeLeft_ -= Time::DeltaTime();
	if (timeLeft_ <= 0.0f)
	{
		timeLeft_ = 0.0f;
		DestroyMe();
	}
}

void BlockEffect::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(Ease::InBounce(timeLeft_ / EFFECT_TIME) * UINT8_MAX));

	Rectan drawRect{ GetRect() };
	for (int i = 0; i < FRAME_WIDTH; i++)
	{
		DrawBox(
			drawRect.x - i, drawRect.y - i,
			drawRect.x + drawRect.width + i, drawRect.y + drawRect.height + i,
			color_, FALSE);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

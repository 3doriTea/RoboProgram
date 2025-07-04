#include "StageObject.h"
#include "Stage.h"
#include "Utility/RectanUtility.h"


namespace
{
	static const int IMAGE_SIZE{ 80 };
}

StageObject::StageObject(const Vector2& _position, const char* _imageFileName)
{
	rect_.pivot = _position;
	rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	hImage_ = LoadGraph(_imageFileName);
	assert(hImage_ != 0
		&& "‰æ‘œ“Ç‚Ýž‚Ý‚ÉŽ¸”s @Flag::Flag");
}

StageObject::~StageObject()
{
}

void StageObject::Update()
{
//	if (RectanUtility::IsHit(rect_.ToInt(), GetPlayer()->GetRectWorld().ToInt()))
//	DrawBox(GetRect().ToInt().x, GetRect().ToInt().y, GetRect().ToInt().x + GetRect().ToInt().width, GetRect().ToInt().y + GetRect().ToInt().height, 0xffff00, TRUE);
//	DrawBox(GetPlayer()->GetRect().ToInt().x, GetPlayer()->GetRect().ToInt().y, GetPlayer()->GetRect().ToInt().x + GetPlayer()->GetRect().ToInt().width, GetPlayer()->GetRect().ToInt().y + GetPlayer()->GetRect().ToInt().height, 0xffff00, TRUE);
	if (RectanUtility::IsHit(GetRect().ToInt(), GetPlayer()->GetRect().ToInt()))
	{
		OnPlayer();
	}
}

Player* StageObject::GetPlayer()
{
	return pStage_->GetPlayer();
}

void StageObject::OnPlayer()
{
}

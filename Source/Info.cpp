#include "Info.h"
#include <cassert>


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Info.png" };
	static const int IMAGE_SIZE{ 80 };
}

Info::Info(const Vector2& _position, const int _infoLevel) :
	StageObject{ _position, IMAGE_FILE },
	INFO_LEVEL_{ _infoLevel }
{
	//rect_.pivot = _position;
	//rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	/*hImage_ = LoadGraph(IMAGE_FILE);
	assert(hImage_ != 0
		&& "‰æ‘œ“Ç‚Ýž‚Ý‚ÉŽ¸”s @Info::Info");*/
}

Info::~Info()
{
}

void Info::Draw()
{
	if (pStage_->GetInfoLevel() >= INFO_LEVEL_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	StageObject::Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Info::OnPlayer()
{
	pStage_->GetInfo(INFO_LEVEL_);
}

#include "Flag.h"
#include <cassert>
#include <cstdint>
#include <cmath>


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Flag.png" };
}

Flag::Flag(const Vector2& _position) :
	StageObject{ _position, IMAGE_FILE }
{
	
}

Flag::~Flag()
{
}

void Flag::Draw()
{
	if ((rect_.pivot - pStage_->GetCheckPoint()).Size() <= FLT_EPSILON)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	StageObject::Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Flag::OnPlayer()
{
	pStage_->SetCheckPoint(rect_.pivot);
}

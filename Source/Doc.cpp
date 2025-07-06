#include "Doc.h"
#include <cassert>


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Doc.png" };
	static const int IMAGE_SIZE{ 80 };
}

Doc::Doc(const Vector2& _position, const int _docLevel) :
	StageObject{ _position, IMAGE_FILE },
	DOC_LEVEL_{ _docLevel }
{
	//rect_.pivot = _position;
	//rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	/*hImage_ = LoadGraph(IMAGE_FILE);
	assert(hImage_ != 0
		&& "‰æ‘œ“Ç‚Ýž‚Ý‚ÉŽ¸”s @Doc::Doc");*/
}

Doc::~Doc()
{
}

void Doc::Draw()
{
	if (pStage_->GetDocLevel() >= DOC_LEVEL_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	StageObject::Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Doc::OnPlayer()
{
	pStage_->GetDocument(DOC_LEVEL_);
}

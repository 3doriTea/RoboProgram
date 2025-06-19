#include "Background.h"
#include <cassert>


namespace
{
	const char IMAGE_PATH[]{ "Data/Image/Background.png" };
}

Background::Background()
{
	hImage_ = LoadGraph(IMAGE_PATH);
	assert(hImage_ >= 0
		&& "”wŒi‰æ‘œ‚Ì“Ç‚İ‚İ‚É¸”s @Background::Background");
}

Background::~Background()
{
}

void Background::Update()
{
}

void Background::Draw()
{
	// TODO: “®‚­‰_‚Ì•`‰æ‚Æ‚©

	DrawGraph(0, 0, hImage_, TRUE);
}

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
		&& "�w�i�摜�̓ǂݍ��݂Ɏ��s @Background::Background");
}

Background::~Background()
{
}

void Background::Update()
{
}

void Background::Draw()
{
	// TODO: �����_�̕`��Ƃ�

	DrawGraph(0, 0, hImage_, TRUE);
}

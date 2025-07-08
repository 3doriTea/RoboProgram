#include "Background.h"
#include <cassert>


Background::Background(const char* _imageFile)
{
	hImage_ = LoadGraph(_imageFile);
	assert(hImage_ >= 0
		&& "�w�i�摜�̓ǂݍ��݂Ɏ��s @Background::Background");

	SetDrawOrder(100);
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

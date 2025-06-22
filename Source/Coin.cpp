#include "Coin.h"
#include <cassert>


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Coin.png" };
	static const int IMAGE_SIZE{ 80 };
}

Coin::Coin(const Vector2& _position)
{
	rect_.pivot = _position;
	rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	//hImage_ = LoadGraph(IMAGE_FILE);
	//assert(hImage_ != 0
	//	&& "âÊëúì«Ç›çûÇ›Ç…é∏îs @Coin::Coin");
}

Coin::~Coin()
{
}

void Coin::Update()
{
}

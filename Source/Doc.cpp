#include "Doc.h"
#include <cassert>


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Doc.png" };
	static const int IMAGE_SIZE{ 80 };
}

Doc::Doc(const Vector2& _position)
{
	rect_.pivot = _position;
	rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	hImage_ = LoadGraph(IMAGE_FILE);
	assert(hImage_ != 0
		&& "âÊëúì«Ç›çûÇ›Ç…é∏îs @Doc::Doc");
}

Doc::~Doc()
{
}

void Doc::Update()
{
}

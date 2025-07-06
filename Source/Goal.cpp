#include "Goal.h"
#include <cassert>
#include "Fader.h"


namespace
{
	static const char IMAGE_FILE[]{ "Data/Image/Goal.png" };
	static const int IMAGE_SIZE{ 80 };
	static const float TO_CLEAR_SCENE_TIME{ 5.0f };
	static const char FADE_IMAGE_FILE[]{ "Data/Image/ClearBackground.png" };
}

Goal::Goal(const Vector2& _position) :
	StageObject{ _position, IMAGE_FILE }
{
	//rect_.pivot = _position;
	//rect_.size = { IMAGE_SIZE, IMAGE_SIZE };

	/*hImage_ = LoadGraph(IMAGE_FILE);
	assert(hImage_ != 0
		&& "�摜�ǂݍ��݂Ɏ��s @Goal::Goal");*/
}

Goal::~Goal()
{
}

void Goal::Draw()
{
	StageObject::Draw();
}

void Goal::OnPlayer()
{
	GetPlayer()->ShockDown();  // �v���C����shock���ēd�����Ƃ�
	(new Fader{ FADE_IMAGE_FILE, TO_CLEAR_SCENE_TIME, false })->OnFinish([&, this]()
		{
			SceneManager::ReloadScene();
		});
}

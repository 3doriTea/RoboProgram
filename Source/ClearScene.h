#pragma once
#include "../Library/SceneBase.h"

class ClearScene : public SceneBase
{
public:
	ClearScene();
	~ClearScene();
	void Update() override;
	void Draw() override;

	void OpenFile(const char* _name);

public:
};

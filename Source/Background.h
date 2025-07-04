#pragma once
#include "../Library/GameObject.h"


class Background : public GameObject
{
public:
	Background(const char* _imageFile);
	~Background();

	void Update() override;
	void Draw() override;

private:
	int hImage_;
};

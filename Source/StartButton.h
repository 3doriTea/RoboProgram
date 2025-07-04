#pragma once
#include "Object2D.h"
#include "Fader.h"


class StartButton : public Object2D
{
public:
	StartButton(const char* _imageFile);
	~StartButton();

	inline void SetPosition(const Vector2Int _position) { rect_.pivot = _position; }
	
	void Update() override;
	void Draw() override;

private:
	int alpha_;
	bool pushed_;
};

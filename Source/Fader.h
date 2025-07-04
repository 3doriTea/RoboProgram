#pragma once
#include "Object2D.h"
#include <functional>


class Fader : public Object2D
{
public:
	enum struct FadeType
	{
		Ghost,
	};
public:
	Fader(const char* _imageFileName, const float _fadeTime, const bool _showToHide);
	~Fader();

	inline void OnFinish(std::function<void()> _callback) { onFinish_ = _callback; }

	void Update() override;
	void Draw() override;

private:
	const float FADE_TIME;
	float timeLeft_;
	FadeType fadeType_;
	bool showToHide_;
	std::function<void()> onFinish_;
};

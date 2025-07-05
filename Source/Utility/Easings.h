#pragma once
#include <cmath>

class Ease
{
public:
	static inline float OutElastic(float x)
	{
		static const float PI{ std::acosf(-1) };
		static const float c4 = (2.0f * PI) / 3.0f;

		return x == 0
		  ? 0
		  : x == 1
		  ? 1
		  : std::powf(2.0f, -10.0f * x) * std::sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
	}

	static inline float InOutElastic(float x)
	{
		static const float PI{ std::acosf(-1) };
		static const float c5 = (2.0f * PI) / 4.5;

		return x == 0
		  ? 0
		  : x == 1
		  ? 1
		  : x < 0.5
		  ? -(std::powf(2.0f, 20.0f * x - 10.0f) * std::sinf((20.0f * x - 11.125f) * c5)) / 2.0f
		  : (std::powf(2.0f, -20.0f * x + 10.0f) * std::sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	static inline float OutCirc(float x)
	{
		return std::sqrtf(1.0f - std::powf(x - 1.0f, 2.0f));
	}

	static inline float OutExpo(float x)
	{
		return x == 1.0f ? 1.0f : 1.0f - std::powf(2.0f, -10 * x);
	}

	static inline float InBounce(float x)
	{
		return 1 - OutBounce(1 - x);
	}

	//  REF: https://easings.net/ja#easeOutBounce
	static inline float OutBounce(float x)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (x < 1.f / d1)
		{
			return n1 * x * x;
		}
		else if (x < 2.f / d1)
		{
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		else if (x < 2.5f / d1)
		{
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		else
		{
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}
	}
	static inline float InOutBounce(float x)
	{
		return x < 0.5
			? (1 - OutBounce(1 - 2 * x)) / 2
			: (1 + OutBounce(2 * x - 1)) / 2;
	}
};

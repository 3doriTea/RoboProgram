#pragma once
#include <DxLib.h>
#include <bitset>

static const size_t KEY_SIZE{ 256 };

class Input
{
public:
	void Update();

private:
	Input();
	~Input();

	std::bitset<KEY_SIZE> current_;
	std::bitset<KEY_SIZE> previous_;

	int currentMouse_;
	int previousMouse_;

public:
	static Input* Instance();

	static bool IsMouse(int _mouseCode);
	static bool IsMouseDown(int _mouseCode);
	static bool IsMouseUp(int _mouseCode);

	static bool IsKey(int _keyCode);
	static bool IsKeyDown(int _keyCode);
	static bool IsKeyUp(int _keyCode);

	static void Release();

private:
	static Input* instance_;
};

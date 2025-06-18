#include "Input.h"

void Input::Update()
{
	static char keyBuffer[KEY_SIZE]{};

	GetHitKeyStateAll(keyBuffer);

	previous_ = current_;

	for (int i = 0; i < KEY_SIZE; i++)
	{
		current_[i] = keyBuffer[i];
	}

	previousMouse_ = currentMouse_;
	currentMouse_ = GetMouseInput();
}

Input::Input()
{
}

Input::~Input()
{
}

Input& Input::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Input{};
	}

	return *instance_;
}

bool Input::IsMouse(int _mouseCode)
{
	return Instance().currentMouse_ & _mouseCode;
}

bool Input::IsMouseDown(int _mouseCode)
{
	int mouseXor{ Instance().currentMouse_ ^ Instance().previousMouse_ };
	return static_cast<bool>(mouseXor & Instance().currentMouse_ & _mouseCode);
}

bool Input::IsMouseUp(int _mouseCode)
{
	int mouseXor{ Instance().currentMouse_ ^ Instance().previousMouse_ };
	return static_cast<bool>(mouseXor & Instance().previousMouse_ & _mouseCode);
}

bool Input::IsKey(int _keyCode)
{
	return Instance().current_[_keyCode];
}

bool Input::IsKeyDown(int _keyCode)
{
	int keyXOr { Instance().current_[_keyCode] ^ Instance().previous_[_keyCode] };
	return static_cast<bool>(keyXOr & Instance().current_[_keyCode]);
}

bool Input::IsKeyUp(int _keyCode)
{
	int keyXOr{ Instance().current_[_keyCode] ^ Instance().previous_[_keyCode] };
	return static_cast<bool>(keyXOr & Instance().previous_[_keyCode]);
}

void Input::Release()
{
	// SAFE_DELETE
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

Input* Input::instance_{ nullptr };

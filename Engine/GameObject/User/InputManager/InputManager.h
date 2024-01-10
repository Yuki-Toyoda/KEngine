#pragma once
#include "../../../Input/Input.h"
#include "../../../Math/Vector3.h"
class InputManager
{
public:
	static bool MoveLeft();
	static bool MoveUp();
	static bool MoveDown();
	static bool MoveRight();
	static Vector3 Move();
	static bool RotateRight();
	static bool RotateLeft();
	static bool ChangeRotate();
	static bool Atack();
	static void Init();
	static void Update();
private:
	static XINPUT_STATE joyState_;
	static XINPUT_STATE preJoyState_;
};


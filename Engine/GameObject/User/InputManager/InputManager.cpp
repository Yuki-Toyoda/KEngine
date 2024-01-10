#include "InputManager.h"

bool InputManager::MoveLeft()
{
    if (Input::GetInstance()->PushKey(DIK_LEFT)) {
        return true;
    }
    return false;
}

bool InputManager::MoveUp()
{
    if (Input::GetInstance()->PushKey(DIK_UP)) {
        return true;
    }
    return false;
}

bool InputManager::MoveDown()
{
    if (Input::GetInstance()->PushKey(DIK_DOWN)) {
        return true;
    }
    return false;
}

bool InputManager::MoveRight()
{
    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
        return true;
    }
    return false;
}

Vector3 InputManager::Move()
{
    Vector3 velocity = {0.0f,0.0f,0.0f};
   // XINPUT_STATE joyState;
    
    velocity = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
       (float)joyState_.Gamepad.sThumbLY / SHRT_MAX,0.0f };
    if (Input::GetInstance()->PushKey(DIK_LEFT)) {
        velocity.x = -1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
        velocity.x = 1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_UP)) {
        velocity.y = 1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_DOWN)) {
        velocity.y = -1.0f;
    }
   
   


    velocity.z = 0.0f;
    return velocity;
}

bool InputManager::RotateRight()
{
    if (Input::GetInstance()->PushKey(DIK_D)) {
        return true;
    }
    return false;
}

bool InputManager::RotateLeft()
{
    if (Input::GetInstance()->PushKey(DIK_A)) {
        return true;
    }
    return false;
}

bool InputManager::ChangeRotate()
{
    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
        !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
        return true;
    }
    return false;
}

bool InputManager::Atack()
{
    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
        !(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
        return true;
    }
    return false;
}

void InputManager::Init()
{
    Input::GetInstance()->GetJoystickState(0, joyState_); // 現在フレームの入力取得
    preJoyState_ = joyState_;
}

void InputManager::Update()
{
    preJoyState_ = joyState_;
    Input::GetInstance()->GetJoystickState(0, joyState_);
}


XINPUT_STATE InputManager::joyState_;
XINPUT_STATE InputManager::preJoyState_;
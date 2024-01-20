#include "InputManager.h"

void InputManager::Init()
{
    // 現在フレームの入力取得
    Input::GetInstance()->GetJoystickState(0, joyState_);
    // 取得した入力を代入
    preJoyState_ = joyState_;
}

void InputManager::Update()
{
    // 前入力を取得
    preJoyState_ = joyState_;
    // 現在入力を取得
    Input::GetInstance()->GetJoystickState(0, joyState_);
}

//bool InputManager::MoveLeft()
//{
//    if (Input::GetInstance()->PushKey(DIK_LEFT)) {
//        return true;
//    }
//    return false;
//}
//
//bool InputManager::MoveUp()
//{
//    if (Input::GetInstance()->PushKey(DIK_UP)) {
//        return true;
//    }
//    return false;
//}
//
//bool InputManager::MoveDown()
//{
//    if (Input::GetInstance()->PushKey(DIK_DOWN)) {
//        return true;
//    }
//    return false;
//}
//
//bool InputManager::MoveRight()
//{
//    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
//        return true;
//    }
//    return false;
//}

Vector3 InputManager::Move(Vector3 velocity)
{
    // 移動方向ベクトル初期化
  //  Vector3 velocity = {0.0f,0.0f,0.0f};
   // XINPUT_STATE joyState;
    
    // スティックの入力によって移動
    velocity +=  Vector3( (float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
       0.0f,(float)joyState_.Gamepad.sThumbLY / SHRT_MAX );
    
    // キー入力によって移動する
    if (Input::GetInstance()->PushKey(DIK_LEFT)) {
        velocity.x = -1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
        velocity.x = 1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_UP)) {
        velocity.z = 1.0f;
    }
    if (Input::GetInstance()->PushKey(DIK_DOWN)) {
        velocity.z = -1.0f;
    }
    if (velocity.x >= 10.0f) {
        velocity.x = 10.0f;
   }
    else if (velocity.x <=- 10.0f) {
       velocity.x = -10.0f;
   }if (velocity.z >= 10.0f) {
       velocity.z = 10.0f;
   }
   else if (velocity.z <= -10.0f) {
       velocity.z = -10.0f;
   }
    // 移動ベクトルを削除する
    return velocity;
}

//bool InputManager::RotateRight()
//{
//
//    if (Input::GetInstance()->PushKey(DIK_D)) {
//        return true;
//    }
//    return false;
//}
//
//bool InputManager::RotateLeft()
//{
//    if (Input::GetInstance()->PushKey(DIK_A)) {
//        return true;
//    }
//    return false;
//}

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


XINPUT_STATE InputManager::joyState_;
XINPUT_STATE InputManager::preJoyState_;
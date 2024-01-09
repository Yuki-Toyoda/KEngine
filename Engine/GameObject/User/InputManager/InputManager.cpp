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


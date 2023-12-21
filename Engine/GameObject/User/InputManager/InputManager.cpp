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



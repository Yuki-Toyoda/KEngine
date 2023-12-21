#include "InputManager.h"

bool InputManager::MoveLeft()
{
    if (Input::GetInstance()->PushKey(DIK_A)) {
        return true;
    }
    return false;
}

bool InputManager::MoveUp()
{
    if (Input::GetInstance()->PushKey(DIK_W)) {
        return true;
    }
    return false;
}

bool InputManager::MoveDown()
{
    if (Input::GetInstance()->PushKey(DIK_S)) {
        return true;
    }
    return false;
}

bool InputManager::MoveRight()
{
    if (Input::GetInstance()->PushKey(DIK_D)) {
        return true;
    }
    return false;
}

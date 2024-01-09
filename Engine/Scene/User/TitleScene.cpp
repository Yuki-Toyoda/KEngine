#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleScene::Init()
{
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
}

void TitleScene::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// Aボタンを押すとゲームシーンへ
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}

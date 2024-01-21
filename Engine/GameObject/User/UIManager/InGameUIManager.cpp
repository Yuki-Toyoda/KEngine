#include "InGameUIManager.h"
#include "../../../Resource/Texture/TextureManager.h"

void InGameUIManager::Init()
{
	// スプライトの追加
	AddSprite("PauseMenu", { 0.0f, 0.0f }, { 224.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "pauseBotton.png"));
	AddSprite("Attack", { 1040.0f, 65.0f }, { 224.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "attack.png"));
	AddSprite("MoveUI_BG", { 1075.0f, 0.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveBack.png"));
	AddSprite("MoveUI_F", { 1075.0f, 0.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveFront.png"));

	// スプライトの描画範囲設定
	sprites_[0]->texSize_ = { 448.0f, 128.0f };
	sprites_[1]->texSize_ = { 448.0f, 96.0f };

	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
}

void InGameUIManager::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	
	// スティックの方向を元に移動ベクトルを求める
	Vector3 move = {
		(float)joyState_.Gamepad.sThumbLX, 0.0f,
		(float)-joyState_.Gamepad.sThumbLY };

	// 正規化
	move = Math::Normalize(move) * 10.0f;

	// スティックの結果をUIの反映
	sprites_[3]->translate_ = Vector2(1075.0f, 0.0f) + Vector2(move.x, move.z);

	// Aボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(448.0f, 0.0f);
	}
	else {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(0.0f, 0.0f);
	}

	// Aボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		// UIを変更する
		sprites_[1]->texBase_ = Vector2(448.0f, 0.0f);
	}
	else {
		// UIを変更する
		sprites_[1]->texBase_ = Vector2(0.0f, 0.0f);
	}
}

void InGameUIManager::DisplayImGui()
{
	// スプライト情報があれば
	if (sprites_.size() > 0) {
		ImGui::Text("Sprites");
		ImGui::BeginChild("Sprites", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (Sprite* s : sprites_) {
			s->DisplayImGui();
		}
		ImGui::EndChild();
	}
}

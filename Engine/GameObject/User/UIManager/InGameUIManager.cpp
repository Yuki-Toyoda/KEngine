#include "InGameUIManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../Enemy/Boss.h"
#include "../Player/uribo/uribo.h"

void InGameUIManager::Init()
{
	// スプライトの追加
	AddSprite("PauseMenu", { 0.0f, 0.0f }, { 224.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "pauseBotton.png"));
	AddSprite("Attack", { 1040.0f, 590.0f }, { 224.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "attack.png"));
	AddSprite("MoveUI_BG", { 1075.0f, 520.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveBack.png"));
	AddSprite("MoveUI_F", { 1075.0f, 520.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveFront.png"));
	AddSprite("BossHPGageBG", { 80.0f, 40.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpFram.png"));
	AddSprite("BossHPGage", { 0.0f, 40.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpGauge.png"));
	AddSprite("BossIcon", { 80.0f, 40.0f }, { 48.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "bossIcon.png"));
	AddSprite("PlayerHPFrameBG", { 0.0f, 620.0f }, { 384.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "playerHPFram.png"));
	AddSprite("PlayerIcon", { 0.0f, 100.0f }, { 384.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "playerIconNormal.png"));
	AddSprite("PlayerHPGageBG", { 100.0f, 685.0f }, { 270.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpFram.png"));
	AddSprite("PlayerHPGage", { 100.0f, 100.0f }, { 270.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpGauge.png"));
	AddSprite("Feed", { 1000.0f, 650.0f }, { 256.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "feed.png"));

	// スプライトの描画範囲設定
	sprites_[0]->SetIsActive(false);
	sprites_[0]->texSize_ = { 448.0f, 128.0f };
	sprites_[1]->texSize_ = { 448.0f, 96.0f };
	sprites_[11]->texSize_ = { 512.0f, 96.0f };

	// アンカーポイント設定
	sprites_[4]->anchorPoint_.y = 0.5f;
	sprites_[5]->anchorPoint_.y = 0.5f;
	sprites_[6]->anchorPoint_ = { 0.5f, 0.5f };
	sprites_[9]->anchorPoint_.y = 0.5f;
	sprites_[10]->anchorPoint_.y = 0.5f;

	// 色の設定
	sprites_[9]->color_ = {2.0f, 2.0f, 2.0f, 1.0f};

	// 座標設定
	//sprites_[9]->translate_ = sprites_[7]->translate_;

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
	sprites_[3]->translate_ = Vector2(1075.0f, 520.0f) + Vector2(move.x, move.z);

	// Aボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(448.0f, 0.0f);
	}
	else {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(0.0f, 0.0f);
	}

	// Xボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		// UIを変更する
		sprites_[11]->texBase_ = Vector2(512.0f, 0.0f);
	}
	else {
		// UIを変更する
		sprites_[11]->texBase_ = Vector2(0.0f, 0.0f);
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

	// ゲージ背景とゲージ部分の座標をそろえる
	sprites_[5]->translate_ = sprites_[4]->translate_;
	// プレイヤーも同様
	sprites_[8]->translate_ = sprites_[7]->translate_;
	sprites_[10]->translate_ = sprites_[9]->translate_;

	// ボスが存在する場合
	if (boss_ != nullptr) {
		// ボスのHP残量によってゲージを変動させる
		sprites_[5]->scale_.x = Math::Linear(boss_->GetHP(), 0.0f, 1120.0f, boss_->GetMaxHP());
	}

	// ウリボーが存在する場合
	if (uribo_ != nullptr) {
		sprites_[10]->scale_.x = Math::Linear((float)uribo_->GetHP(), 0.0f, 270.0f, (float)uribo_->GetDefaultHP());
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

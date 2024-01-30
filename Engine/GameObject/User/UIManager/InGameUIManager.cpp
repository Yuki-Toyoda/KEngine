#include "InGameUIManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Boss.h"
#include "../Player/uribo/uribo.h"

void InGameUIManager::Init()
{
	// スプライトの追加
	bossHPGageSprite_BG_ = AddSprite("BossHPGageBG", { 80.0f, 40.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpFram.png"));
	bossHPGageSprite_F_ = AddSprite("BossHPGage", { 0.0f, 0.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpGauge.png"));
	bossHPGageSprite_Icon_ = AddSprite("BossIcon", { 0.0f, 0.0f }, { 48.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "bossIcon.png"));
	playerHPFrame_ = AddSprite("PlayerHPFrame", { 10.0f, 620.0f }, { 256.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "playerHPFram.png"));
	playerIcon_ = AddSprite("PlayerIcon", { 0.0f, 0.0f }, { 384.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "playerIconNormal.png"));
	playerVegetableIcon_ = AddSprite("PlayerVegetables", { 200.0f, 47.0f }, { 80.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "vegetableIcon.png"));
	playerVegetableCount2_ = AddSprite("VegetableCount2", { 190.0f, 47.0f }, { 48.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "vegetableNumbert.png"));
	playerVegetableCount1_ = AddSprite("VegetableCount1", { 138.0f, 47.0f }, { 48.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "vegetableNumbert.png"));
	moveSpriteBG_ = AddSprite("MoveUI_BG", { 1075.0f, 520.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveBack.png"));
	moveSprite_ = AddSprite("MoveUI", { 0.0f, 0.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveFront.png"));
	AttackSprite_ = AddSprite("Attack", { 1040.0f, 590.0f }, { 224.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "attack.png"));
	FeedSprite_ = AddSprite("Feed", { 1000.0f, 650.0f }, { 256.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "feed.png"));

	// アンカーポイント設定
	bossHPGageSprite_Icon_->anchorPoint_ = { 0.5f, 0.2f };
	playerVegetableIcon_->anchorPoint_ = { 0.5f, 0.5f };
	playerVegetableCount2_->anchorPoint_ = { 0.5f, 0.5f };
	playerVegetableCount1_->anchorPoint_ = { 0.5f, 0.5f };

	// ボスのHPゲージ背景に親子付けする
	bossHPGageSprite_F_->SetParent(bossHPGageSprite_BG_->GetWorldTransform());
	bossHPGageSprite_Icon_->SetParent(bossHPGageSprite_BG_->GetWorldTransform());

	// プレイヤーのHP背景に親子付けする
	playerIcon_->SetParent(playerHPFrame_->GetWorldTransform());
	playerVegetableIcon_->SetParent(playerHPFrame_->GetWorldTransform());
	playerVegetableCount2_->SetParent(playerHPFrame_->GetWorldTransform());
	playerVegetableCount1_->SetParent(playerHPFrame_->GetWorldTransform());

	// 野菜アイコンの角度を設定
	playerVegetableIcon_->rotate_ = 0.65f;

	// 野菜カウント数スプライトのリセット
	playerVegetableCount2_->texSize_ = Vector2(48.0f, 64.0f);
	playerVegetableCount1_->texSize_ = Vector2(48.0f, 64.0f);

	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// スティックUIの背景に親子付けする
	moveSprite_->SetParent(moveSpriteBG_->GetWorldTransform());

	// スプライトの描画範囲設定
	AttackSprite_->texSize_ = { 448.0f, 96.0f };
	FeedSprite_->texSize_ = { 512.0f, 96.0f };
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
	moveSprite_->translate_ = Vector2(move.x, move.z);

	// Aボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		// UIを変更する
		AttackSprite_->texBase_ = Vector2(448.0f, 0.0f);
	}
	else {
		// UIを変更する
		AttackSprite_->texBase_ = Vector2(0.0f, 0.0f);
	}

	// Xボタンを押すとUIを変化させる
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		// UIを変更する
		FeedSprite_->texBase_ = Vector2(512.0f, 0.0f);
	}
	else {
		// UIを変更する
		FeedSprite_->texBase_ = Vector2(0.0f, 0.0f);
	}
	
	// プレイヤーが存在する場合
	if (player_ != nullptr) {
		if (player_->GetAbsorptionCount() >= 10) {
			// プレイヤーの所持野菜数を文字列で取得
			std::string countNum = std::to_string(player_->GetAbsorptionCount());
			// 数字の最初の要素を取得
			std::string count = countNum.substr(0, 1);
			// 一桁目のスプライトの描画位置を変更
			int c = atoi(count.c_str());
			playerVegetableCount1_->texBase_ = Vector2({ (48.0f * (float)c), 0.0f });

			// 数字の2桁目の要素を取得
			count = countNum.substr(1, 1);
			c = atoi(count.c_str());
			playerVegetableCount2_->texBase_ = Vector2({ (48.0f * (float)c), 0.0f });
		}
		else {
			// 一桁目のスプライトの描画位置を変更
			playerVegetableCount1_->texBase_ = Vector2(0.0f,0.0f);

			// プレイヤーの所持野菜数を文字列で取得
			std::string countNum = std::to_string(player_->GetAbsorptionCount());
			// 数字の最初の要素を取得
			std::string count = countNum.substr(0, 1);
			// 一桁目のスプライトの描画位置を変更
			int c = atoi(count.c_str());
			playerVegetableCount2_->texBase_ = Vector2({ (48.0f * (float)c), 0.0f });
		}
	}

	// ボスが存在する場合
	if (boss_ != nullptr) {
		// ボスのHP残量によってゲージを変動させる
		bossHPGageSprite_F_->scale_.x = Math::Linear(boss_->GetHP(), 0.0f, 1120.0f, boss_->GetMaxHP());
	}

	// ウリボーが存在する場合
	if (uribo_ != nullptr) {
		
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

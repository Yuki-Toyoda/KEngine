#include "InGameUIManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../Player/Player.h"
#include "../Enemy/Boss.h"
#include "../Player/uribo/uribo.h"
#include "../../Core/Camera.h"

void InGameUIManager::Init()
{
	// スプライトの追加
	bossHPGageSprite_BG_ = AddSprite("BossHPGageBG", { 80.0f, 40.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpFram.png"));
	bossHPGageSprite_F_ = AddSprite("BossHPGage", { 0.0f, 0.0f }, { 1120.0f, 32.0f }, TextureManager::Load("./Resources/UI/InGame", "BossHpGauge.png"));
	bossHPGageSprite_Icon_ = AddSprite("BossIcon", { 0.0f, 0.0f }, { 48.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "bossIcon.png"));
	playerHPFrame_ = AddSprite("PlayerHPFrame", { 5.0f, 583.0f }, { 256.0f, 140.0f }, TextureManager::Load("./Resources/UI/InGame", "playerHPFram.png"));
	playerVegetableCount2_ = AddSprite("VegetableCount2", { 196.0f, 62.0f }, { 40.0f, 52.0f }, TextureManager::Load("./Resources/UI/InGame", "vegetableNumbert.png"));
	playerVegetableCount1_ = AddSprite("VegetableCount1", { 156.0f, 62.0f }, { 40.0f, 52.0f }, TextureManager::Load("./Resources/UI/InGame", "vegetableNumbert.png"));
	uriboGage_BG_ = AddSprite("UriboGage_BG", { 80.0f, 650.0f }, { 280.0f, 12.0f }, TextureManager::Load("./Resources/UI/InGame", "urimaruHpFram.png"));
	uriboGage_F_ = AddSprite("UriboGage_F", { 80.0f, 650.0f }, { 280.0f, 12.0f }, TextureManager::Load("./Resources/UI/InGame", "urimaruHpGauge.png"));
	uriboIcon_ = AddSprite("UriboIcon", { 80.0f, 650.0f }, { 24.0f, 24.0f }, TextureManager::Load("./Resources/UI/InGame", "UrimaruIconNormal.png"));
	uriboAlert_ = AddSprite("UriboAlert", { 80.0f, 650.0f }, { 96.0f, 96.0f }, TextureManager::Load("./Resources/UI/InGame", "urimaruAlert.png"));

	moveSpriteBG_ = AddSprite("MoveUI_BG", { 1075.0f, 520.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveBack.png"));
	moveSprite_ = AddSprite("MoveUI", { 0.0f, 0.0f }, { 192.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "moveFront.png"));
	AttackSprite_ = AddSprite("Attack", { 1040.0f, 590.0f }, { 224.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "attack.png"));
	FeedSprite_ = AddSprite("Feed", { 1000.0f, 650.0f }, { 256.0f, 48.0f }, TextureManager::Load("./Resources/UI/InGame", "feed.png"));
	moveSpriteBG_->SetIsActive(false);
	moveSprite_->SetIsActive(false);
	AttackSprite_->SetIsActive(false);
	// アンカーポイント設定
	bossHPGageSprite_Icon_->anchorPoint_ = { 0.5f, 0.2f };
	playerVegetableCount2_->anchorPoint_ = { 0.5f, 0.5f };
	playerVegetableCount1_->anchorPoint_ = { 0.5f, 0.5f };
	uriboIcon_->anchorPoint_ = { 0.5f, 0.5f };
	uriboAlert_->anchorPoint_ = { 1.0f, 1.0f };

	// ボスのHPゲージ背景に親子付けする
	bossHPGageSprite_F_->SetParent(bossHPGageSprite_BG_->GetWorldTransform());
	bossHPGageSprite_Icon_->SetParent(bossHPGageSprite_BG_->GetWorldTransform());

	// プレイヤーのHP背景に親子付けする
	playerVegetableCount2_->SetParent(playerHPFrame_->GetWorldTransform());
	playerVegetableCount1_->SetParent(playerHPFrame_->GetWorldTransform());

	// 野菜カウント数スプライトのリセット
	playerVegetableCount2_->texSize_ = Vector2(48.0f, 64.0f);
	playerVegetableCount1_->texSize_ = Vector2(48.0f, 64.0f);

	// ウリボのアラートアイコンの表示領域を設定
	uriboAlert_->texSize_ = { 96.0f, 96.0f };

	// ウリボゲージ背景親子付けする
	//uriboGage_F_->SetParent(uriboGage_BG_->GetWorldTransform());

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
	isTutrial_ = false;

	// アラートアイコンの切り替えタイマー
	switchAlertIconTimer_.Start(switchAlertIconTime_);
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
		// ウリボのワールド座標の取得
		Vector3 positionUriboWorld = uribo_->bodyTransform_.GetWorldPos();
		// ビューポート行列の生成
		Matrix4x4 matViewPort = Math::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);
		// ビュープロジェクション行列の計算
		Matrix4x4 matViewProjectionMat = camera_->GetViewProjectionMatrix() * matViewPort;

		// ワールド座標をスクリーン座標に返還
		positionUriboWorld = Math::Transform(positionUriboWorld, matViewProjectionMat);
		
		// ウリボのゲージを計算した座標を元に移動させる
		uriboGage_BG_->translate_ = { positionUriboWorld.x - (uriboGage_BG_->scale_.x / 2.0f), (positionUriboWorld.y - 50.0f)};
		uriboGage_F_->translate_ = { positionUriboWorld.x - (uriboGage_BG_->scale_.x / 2.0f), (positionUriboWorld.y - 50.0f)};
		uriboIcon_->translate_ = { positionUriboWorld.x - (uriboGage_BG_->scale_.x / 2.0f), (positionUriboWorld.y - 50.0f) };
		uriboAlert_->translate_ = { positionUriboWorld.x - (uriboGage_BG_->scale_.x / 8.0f), positionUriboWorld.y - (uriboGage_BG_->scale_.y * 2.0f) };

		// ウリボのHP割合を取得
		float uriboHPRatio = (float)uribo_->GetHP() / (float)uribo_->GetDefaultHP();

		// ウリボのHPが3 / 1 以下になったときにアラートアイコンを表示
		if (uriboHPRatio <= 0.33f) {
			uriboAlert_->SetIsActive(true);
		}
		else {
			uriboAlert_->SetIsActive(false);
		}
		// ウリボのHPの残量によってゲージを変える
		uriboGage_F_->scale_.x = Math::Linear((float)uribo_->GetHP(), 0.0f, 280.0f, (float)uribo_->GetDefaultHP());

		// ウリボのアラートアイコンが表示されている場合
		if (uriboAlert_->GetIsActive()) {
			// 切り替えタイマー終了時
			if (switchAlertIconTimer_.GetIsFinish()) {
				if (uriboAlert_->texBase_.x >= 96.0f) {
					uriboAlert_->texBase_.x = 0.0f;
				}
				else {
					uriboAlert_->texBase_.x = 96.0f;
				}

				// アラートアイコン切り替えタイマーのリスタート
				switchAlertIconTimer_.Start(switchAlertIconTime_);
			}

			// アラートアイコン切り替えタイマーの更新
			switchAlertIconTimer_.Update();
		}
	}
	if (isTutrial_) {
		bossHPGageSprite_BG_->SetIsActive(false);
		bossHPGageSprite_F_->SetIsActive(false);
		bossHPGageSprite_Icon_->SetIsActive(false);
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

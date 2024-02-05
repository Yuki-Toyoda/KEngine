#include "Rifle.h"

void Rifle::Init()
{
	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 音再生クラスのインスタンス取得
	audio_ = Audio::GetInstance();
	
	// 効果音ロード
	shotSE_ = audio_->LoadWave("./Resources/Audio/Shot.wav");
	cockSE_ = audio_->LoadWave("./Resources/Audio/Pump.wav");
	dropSE_ = audio_->LoadWave("./Resources/Audio/Drop.wav");
	reloadSE_ = audio_->LoadWave("./Resources/Audio/Reload.wav");

	// 各種トランスフォームの初期化
	transform_.translate_ = { 1.5f, 0.8f, 2.5f };
	transform_.rotate_.y = -(float)std::numbers::pi / 2.0f;
	baseTransform_.Init();
	baseTransform_.SetParent(&transform_);
	forendTransform_.Init();
	forendTransform_.SetParent(&baseTransform_);
	bulletTransform_.Init();
	bulletTransform_.translate_ = { 0.0f, -0.5f, 1.25f };
	bulletTransform_.SetParent(&baseTransform_);

	// メッシュ追加関数
	AddMesh(&baseTransform_, color_, "./Resources/Rifle", "Rifle.obj");
	AddMesh(&forendTransform_, color_, "./Resources/Rifle", "ForEnd.obj");
	AddMesh(&bulletTransform_, color_, "./Resources/ShotShell", "ShotShell.obj");

	// アニメーションマネージャーのインスタンス取得
	animManager_ = AnimationManager::GetInstance();

	// パラメータ生成
	CreateParameter("Idle");
	CreateParameter("Shot");
	CreateParameter("Cocking");
	CreateParameter("Reload");

	// アニメーション生成
	anim_ = animManager_->CreateAnimation("RifleAnim", "Idle");
	// キー生成
	anim_->AddAnimationKeys<Vector3>("Base_Rotate", &baseTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Base_Translate", &baseTransform_.translate_);
	anim_->AddAnimationKeys<float>("Forend_Push", &forendTransform_.translate_.z);
	anim_->AddAnimationKeys<Vector3>("Bullet_Rotate", &bulletTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Bullet_Translate", &bulletTransform_.translate_);

	// ループ有効
	anim_->isLoop_ = true;
	// 再生
	anim_->Play();
}

void Rifle::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 待機状態アニメーションの場合
	if (anim_->GetReadingParameterName() == "Idle") {
		// 弾数1以上
		if (ammoCount_ > 0) {
			// トリガーが押し込まれてたら
			if (joyState_.Gamepad.bRightTrigger >= 100 ||
				input_->IsTriggerMouse(0)) {
				// 射撃
				anim_->ChangeParameter("Shot", true);
				// ループを切る
				anim_->isLoop_ = false;

				// 音再生
				audio_->PlayWave(shotSE_);

				// 弾を減らす
				ammoCount_--;
			}
		}

		// 弾数が1発でも減っていたら
		if (ammoCount_ < maxAmmoCount_) {
			// Xボタンを押したら
			if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
				!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X)) ||
				input_->TriggerKey(DIK_R)) {
				// リロード
				anim_->ChangeParameter("Reload", true);
				// ループを切る
				anim_->isLoop_ = false;
			}
		}
	}

	// 射撃状態アニメーションの場合
	if (anim_->GetReadingParameterName() == "Shot") {
		// アニメーション終了時
		if (anim_->isEnd_) {
			// 弾が残っていたら
			if (ammoCount_ > 0) {
				// コッキング
				anim_->ChangeParameter("Cocking", true);
				// 再生
				anim_->Play();

				// 音再生
				audio_->PlayWave(cockSE_);
			}
			else {

				// 打ち切りリロード
				isNoAmmoReload_ = true;

				// 弾がないときはリロード
				anim_->ChangeParameter("Reload", true);
				// 再生
				anim_->Play();
			}
		}
	}

	// コッキングアニメーションの場合
	if (anim_->GetReadingParameterName() == "Cocking") {
		// アニメーション終了時
		if (anim_->isEnd_) {
			// 待機状態に
			anim_->ChangeParameter("Idle", true);
			// ループ有効
			anim_->isLoop_ = true;
			// 再生
			anim_->Play();

			// 打ち切りリロードではない
			isNoAmmoReload_ = false;

			// 音再生
			audio_->PlayWave(dropSE_);
		}
	}

	// リロードアニメーションの場合
	if (anim_->GetReadingParameterName() == "Reload") {
		// 進捗が5割りを超えたら効果音再生
		if (anim_->GetAnimationProgress() >= 0.5f) {
			if (!isReload_) {
				// 音再生
				audio_->PlayWave(reloadSE_);

				// 残弾+
				ammoCount_++;

				// トリガーtrue
				isReload_ = true;
			}
		}
		
		// アニメーション終了時
		if (anim_->isEnd_) {
			// リロードしてない
			isReload_ = false;

			// 弾が残っていたら
			if (ammoCount_ < maxAmmoCount_) {
				// リロード継続
				anim_->ChangeParameter("Reload", true);
				// 再生
				anim_->Play();
			}
			else {
				if (!isNoAmmoReload_) {
					// 最大弾数の場合待機状態へ
					anim_->ChangeParameter("Idle", true);
					// ループ有効
					anim_->isLoop_ = true;
					// 再生
					anim_->Play();
				}
				else {
					// コッキング
					anim_->ChangeParameter("Cocking", true);
					// 再生
					anim_->Play();

					// 音再生
					audio_->PlayWave(cockSE_);
				}		
			}
		}
	}
}

void Rifle::DisplayImGui()
{
	// トランスフォームのImGuiを表示
	transform_.DisplayImGui();
	baseTransform_.DisplayImGuiWithTreeNode("Base");
	forendTransform_.DisplayImGuiWithTreeNode("ForEnd");
	bulletTransform_.DisplayImGuiWithTreeNode("Bullet");

	// 弾数表示
	ImGui::DragInt("Ammo", &ammoCount_, 0);

	// アニメーションのImGuiを表示
	anim_->DisplayImGui();
	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("ChangeReadParameter")) {
		if (ImGui::Button("Idle")) {
			anim_->ChangeParameter("Idle", true);
		}
		if (ImGui::Button("Shot")) {
			anim_->ChangeParameter("Shot", true);
		}
		if (ImGui::Button("Cocking")) {
			anim_->ChangeParameter("Cocking", true);
		}
		if (ImGui::Button("Reload")) {
			anim_->ChangeParameter("Reload", true);
		}
		ImGui::TreePop();
	}

}

void Rifle::CreateParameter(const std::string& name)
{
	// パラメータ生成
	animManager_->CreateAnimationParameter(name);
	// キー生成
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Base_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Base_Translate");
	animManager_->AddSelectAnimationKeys<float>(name, "Forend_Push");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Bullet_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Bullet_Translate");
}

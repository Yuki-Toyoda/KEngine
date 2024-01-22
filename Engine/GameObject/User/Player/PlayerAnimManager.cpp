#include "PlayerAnimManager.h"
#include "Player.h"

void PlayerAnimManager::Init()
{
	// 各トランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);

	// メッシュ追加
	// デバッグ時のみ
#ifdef _DEBUG

	// デバッグ時でも一応プレイヤー描画
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");

#endif // _DEBUG

	// リリースビルドの時
#ifndef _DEBUG

	// 身体
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");

#endif // _RELEASE

	// アニメーションマネージャ取得
	animManager_ = AnimationManager::GetInstance();

	/// パラメータ作成
	// タイトル上の待機状態
	CreateParameter("Player_Idle");

	// ダメージアニメーション
	CreateParameter("Player_Damage");
	// ダメージ中アニメーション
	CreateParameter("Player_Damaging");
	// ダメージからの復帰アニメーション
	CreateParameter("Player_DamageRecovery");

	// スタンアニメーション
	CreateParameter("Player_Stun");
	// スタン中アニメーション
	CreateParameter("Player_Stuning");
	// スタン復帰アニメーション
	CreateParameter("Player_StunRecovery");
}

void PlayerAnimManager::Update()
{
	// アニメーションがnullptr以外だったら
	if (anim_ != nullptr) {
		// ダメージの開始アニメーション中
		if (anim_->GetReadingParameterName() == "Player_Damage") {
			// アニメーション終了してたら
			if (anim_->isEnd_) {
				// ダメージ中アニメーションに遷移
				anim_->ChangeParameter("Player_Damaging", true);
				// ループ有効
				anim_->isLoop_ = true;
			}
		}
		// ダメージ中のアニメーション中
		if (anim_->GetReadingParameterName() == "Player_Damaging") {
			// タイマー終了してたら
			if (stanTimer_.GetIsFinish()) {
				// ダメージ後の復帰のアニメーションに遷移
				anim_->ChangeParameter("Player_DamageRecovery", true);
				// ループ無効
				anim_->isLoop_ = false;
			}

			// スタン秒数タイマー更新
			stanTimer_.Update();

		}
		// ダメージからの復帰アニメーション中
		if (anim_->GetReadingParameterName() == "Player_DamageRecovery") {
			// アニメーション終了してたら
			if (anim_->isEnd_) {
				// ダメージ後の復帰のアニメーションに遷移
				anim_->ChangeParameter("Player_Idle", true);
				// ループ有効
				anim_->isLoop_ = true;
			}
		}
	}
}

void PlayerAnimManager::DisplayImGui()
{
	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

	transform_.DisplayImGuiWithTreeNode("PamTranslate");
	// 各種パーツの情報表示
	bodyTransform_.DisplayImGuiWithTreeNode("body");

	// アニメーションのImGui
	if (anim_ != nullptr) {
		anim_->DisplayImGui();
		// アニメーションの読み込みパラメータ変更
		if (ImGui::TreeNode("Player_ChangeReadParameter")) {
			if (ImGui::Button("Idle")) {
				anim_->ChangeParameter("Player_Idle", true);
			}
			if (ImGui::Button("Damage")) {
				anim_->ChangeParameter("Player_Damage", true);
			}
			if (ImGui::Button("Damaging")) {
				anim_->ChangeParameter("Player_Damaging", true);
			}
			if (ImGui::Button("DamageRecovery")) {
				anim_->ChangeParameter("Player_DamageRecovery", true);
			}
			if (ImGui::Button("Stun")) {
				anim_->ChangeParameter("Player_Stun", true);
			}
			if (ImGui::Button("Stuning")) {
				anim_->ChangeParameter("Player_Stuning", true);
			}
			if (ImGui::Button("StunRecovery")) {
				anim_->ChangeParameter("Player_StunRecovery", true);
			}

			ImGui::TreePop();
		}

		stanTimer_.DisplayImGui("StanTimer");

		// ダメージ処理
		ImGui::DragFloat("AnimationTime", &imGuiAnimationTime_, 0.01f, 0.01f, 5.0f);
		if (ImGui::Button("Damage")) {
			// ダメージ
			Damage(imGuiAnimationTime_);
		}
	}
}

void PlayerAnimManager::SetPlayer(Player* player)
{
	// プレイヤーをセット
	player_ = player;
	// プレイヤーの座標に追従させる
	transform_.SetParent(&player_->transform_);
}

void PlayerAnimManager::CreateParameter(const std::string& name)
{
	// パラメータ生成
	animManager_->CreateAnimationParameter(name);
	// キー生成
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
}

void PlayerAnimManager::CrateAnimation()
{
	// アニメーション生成
	anim_ = animManager_->CreateAnimation("PlayerAnim", "Player_Idle");
	anim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	anim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);

	// アニメーションループtrue
	anim_->isLoop_ = true;
	// アニメーション再生
	anim_->Play();
}

void PlayerAnimManager::ChangeParamameter(const std::string& name, const bool& isChanged)
{
	// パラメータ変更
	anim_->ChangeParameter(name, isChanged);
}

void PlayerAnimManager::Damage(const float& stanTime)
{
	// スタンタイマー開始
	stanTimer_.Start(stanTime);
	// 読み込みパラメータ変更
	anim_->ChangeParameter("Player_Damage", true);
	// ループ無効
	anim_->isLoop_ = false;
}

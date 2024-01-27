#include "BossAnimManager.h"
#include "Boss.h"

void BossAnimManager::Init()
{
	// 各種トランスフォームの初期化
	// 身体
	bodyTransform_.Init();
	bodyTransform_.translate_ = { 0.0f, 0.4f, 0.0f };
	bodyTransform_.SetParent(&transform_);
	// 左腕
	armTransform_L_.Init();
	armTransform_L_.translate_ = {0.4f, 0.7f, 0.0f};
	armTransform_L_.SetParent(&bodyTransform_);
	// 右腕
	armTransform_R_.Init();
	armTransform_R_.translate_ = {-0.4f, 0.7f, 0.0f};
	armTransform_R_.SetParent(&bodyTransform_);
	// 左足
	footTransform_L_.Init();
	footTransform_L_.translate_ = {0.22f, -.1f, 0.0f};
	footTransform_L_.SetParent(&transform_);
	// 右足
	footTransform_R_.Init();
	footTransform_R_.translate_ = {-0.22f, -.1f, 0.0f};
	footTransform_R_.SetParent(&transform_);

	// メッシュ追加
	// デバッグ時のみ
#ifdef _DEBUG

	// 身体
	AddMesh(&bodyTransform_, color_, "./Resources/Boss", "Body.obj");
	// 左腕
	AddMesh(&armTransform_L_, color_, "./Resources/Boss", "Arm_L.obj");
	// 右腕
	AddMesh(&armTransform_R_, color_, "./Resources/Boss", "Arm_R.obj");
	// 左足
	AddMesh(&footTransform_L_, color_, "./Resources/Boss", "Foot_L.obj");
	// 右足
	AddMesh(&footTransform_R_, color_, "./Resources/Boss", "Foot_R.obj");

#endif // _DEBUG

	// リリースビルド時のみ
#ifndef _DEBUG

	// 身体
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");

#endif // _RELEASE

	// アニメーションマネージャ取得
	animManager_ = AnimationManager::GetInstance();

	/// パラメータ生成
	// 待機状態
	CreateParameter("Boss_Idle");
	// ダメージ喰らった
	CreateParameter("Boss_Damage");
	// 倒れる
	CreateParameter("Boss_Dead");
	// 落下攻撃
	CreateParameter("Boss_StartFallAttack");
	CreateParameter("Boss_FallAttacking");
	CreateParameter("Boss_EndFallAttack");

	// 突き上げ攻撃
	CreateParameter("Boss_StartThrustUpAttack");
	CreateParameter("Boss_ThrustUpAttacking");
	CreateParameter("Boss_EndThrustUpAttack");

	// ローラー攻撃右
	CreateParameter("Boss_StartRollerAttack");
	CreateParameter("Boss_RollerAttacking");
	CreateParameter("Boss_EndRollerAttack");


}

void BossAnimManager::Update()
{
	// ダメージアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_Damage") {
		// アニメーションが終了している場合
		if (anim_->isEnd_) {
			// アニメーションのループ設定を有効
			anim_->isLoop_ = true;
			// に再生パラメータを変更
			anim_->ChangeParameter("Boss_Idle", true);
		}
	}

	// 落下攻撃開始のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_StartFallAttack") {
		// アニメーションが終了している場合
		if (anim_->isEnd_) {
			// アニメーションのループ設定を有効
			anim_->isLoop_ = true;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_FallAttacking", true);
		}
	}
	// 落下攻撃準備中のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_FallAttacking") {
		// 複数落下アニメーション再生中かつ、終了トリガーがtrueの場合
		if (isMultiFalling_ && isMultiFallEnd_) {
			// アニメーションのループ設定を有効
			anim_->isLoop_ = true;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_Idle", true);

			// 複数落下アニメーション再生中ではない
			isMultiFalling_ = false;
		}
		
		// アニメーションタイマーが終了しているかつ、複数落下アニメーション再生中でない場合
		if (animTimer_.GetIsFinish() && !isMultiFalling_) {
			// アニメーションのループ設定を無効
			anim_->isLoop_ = false;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_EndFallAttack", true);
		}

		// アニメーションタイマー更新
		animTimer_.Update();
	}
	// 落下攻撃終了中のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_EndFallAttack") {
		// アニメーションが終了している場合
		if (anim_->isEnd_) {
			// 複数落下アニメーション再生トリガーがtrueの場合
			if (playMultiFallAttackAnim_) {
				// アニメーションのループ設定を有効
				anim_->isLoop_ = true;
				// 再生パラメータを変更
				anim_->ChangeParameter("Boss_FallAttacking", true);

				// 再生トリガーはfalseに
				playMultiFallAttackAnim_ = false;
			}
			else {
				// アニメーションのループ設定を有効
				anim_->isLoop_ = true;
				// 再生パラメータを変更
				anim_->ChangeParameter("Boss_Idle", true);
			}
		}
	}

	// 突き上げ攻撃開始のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_StartThrustUpAttack") {
		// アニメーションが終了している場合
		if (anim_->isEnd_) {
			// アニメーションのループ設定を有効
			anim_->isLoop_ = true;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_ThrustUpAttacking", true);
		}
	}
	// 突き上げ攻撃準備中のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_ThrustUpAttacking") {
		// アニメーションタイマーが終了している場合
		if (animTimer_.GetIsFinish()) {
			// アニメーションのループ設定を無効
			anim_->isLoop_ = false;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_EndThrustUpAttack", true);
		}

		// アニメーションタイマー更新
		animTimer_.Update();
	}
	// 突き上げ攻撃終了中のアニメーション再生中の場合
	if (anim_->GetReadingParameterName() == "Boss_EndThrustUpAttack") {
		// アニメーションが終了している場合
		if (anim_->isEnd_) {
			// アニメーションのループ設定を有効
			anim_->isLoop_ = true;
			// 再生パラメータを変更
			anim_->ChangeParameter("Boss_Idle", true);
		}
	}
}

void BossAnimManager::DisplayImGui()
{
	// アニメーションのImGui
	if (anim_ != nullptr) {
		anim_->DisplayImGui();
		// アニメーションの読み込みパラメータ変更
		if (ImGui::TreeNode("Boss_ChangeReadParameter")) {
			if (ImGui::Button("Idle")) {
				anim_->ChangeParameter("Boss_Idle", true);
			}
			if (ImGui::Button("Damage")) {
				anim_->ChangeParameter("Boss_Damage", true);
			}
			if (ImGui::Button("Dead")) {
				anim_->ChangeParameter("Boss_Dead", true);
			}
			if (ImGui::Button("FallAttackStart")) {
				anim_->ChangeParameter("Boss_StartFallAttack", true);
			}
			if (ImGui::Button("FallAttacking")) {
				anim_->ChangeParameter("Boss_FallAttacking", true);
			}
			if (ImGui::Button("EndFallAttack")) {
				anim_->ChangeParameter("Boss_EndFallAttack", true);
			}
			if (ImGui::Button("StartThrustUpAttack")) {
				anim_->ChangeParameter("Boss_StartThrustUpAttack", true);
			}
			if (ImGui::Button("ThrustUpAttacking")) {
				anim_->ChangeParameter("Boss_ThrustUpAttacking", true);
			}
			if (ImGui::Button("EndThrustUpAttack")) {
				anim_->ChangeParameter("Boss_EndThrustUpAttack", true);
			}
			ImGui::TreePop();
		}

		// ボタンを押すと落下攻撃アニメーションを再生
		if (ImGui::Button("FallAttack")) {
			PlayFallAttackAnim(fallAttackReadyTime_);
		}

		// ボタンを押すと突き上げ攻撃アニメーションを再生
		if (ImGui::Button("PushUpAttack")) {
			PlayPushUpAttackAnim(pushUpReadyTime_);
		}
	}

	// 落下攻撃準備時間をImGuiで調整
	ImGui::DragFloat("FallAttackReadyTime", &fallAttackReadyTime_, 0.01f, 0.1f, 3.0f);
	ImGui::DragFloat("PushUpReadyTime", &pushUpReadyTime_, 0.01f, 0.1f, 3.0f);

	transform_.DisplayImGuiWithTreeNode("Eam_Transform");
	// 各種パーツの情報表示
	bodyTransform_.DisplayImGuiWithTreeNode("Body"); // 身体
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L"); // 左腕
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R"); // 右腕
	footTransform_L_.DisplayImGuiWithTreeNode("Foot_L"); // 左足
	footTransform_R_.DisplayImGuiWithTreeNode("Foot_R"); // 右足
}

void BossAnimManager::CreateParameter(const std::string& name)
{
	// パラメータ生成
	animManager_->CreateAnimationParameter(name);
	// キー配列生成
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Foot_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Foot_L_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Foot_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Foot_R_Translate");
}

void BossAnimManager::CreateAnimation()
{
	// アニメーション生成
	anim_ = animManager_->CreateAnimation("Boss_Anim", "Boss_Idle");
	// キー配列生成
	anim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	anim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);
	anim_->AddAnimationKeys<Vector3>("Arm_L_Rotate", &armTransform_L_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Arm_L_Translate", &armTransform_L_.translate_);
	anim_->AddAnimationKeys<Vector3>("Arm_R_Rotate", &armTransform_R_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Arm_R_Translate", &armTransform_R_.translate_);
	anim_->AddAnimationKeys<Vector3>("Foot_L_Rotate", &footTransform_L_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Foot_L_Translate", &footTransform_L_.translate_);
	anim_->AddAnimationKeys<Vector3>("Foot_R_Rotate", &footTransform_R_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Foot_R_Translate", &footTransform_R_.translate_);

	// アニメーションループtrue
	anim_->isLoop_ = true;
	// アニメーション再生
	anim_->Play();
}

void BossAnimManager::SetBoss(Boss* boss)
{
	// ボスをセットする
	boss_ = boss;
	// ボスの座標に追従させる
	transform_.SetParent(&boss_->transform_);
}

void BossAnimManager::ChangeParamameter(const std::string& name, const bool& isChanged)
{
	// パラメータ変更
	anim_->ChangeParameter(name, isChanged);
}

void BossAnimManager::SetPlayMultiFallAttackAnim(const bool isAttack)
{
	// 攻撃トリガーtrue
	playMultiFallAttackAnim_ = isAttack;

	// 落下攻撃アニメーション終了時のパラメータを読み込む
	anim_->ChangeParameter("Boss_EndFallAttack", true);
}

void BossAnimManager::PlayDamageAnim()
{
	// アニメーションのループを無効
	anim_->isLoop_ = false;

	// アニメーションの読み込みパラメータ変更
	anim_->ChangeParameter("Boss_Damage", true);

	/// ダメージアニメーション再生時は各種トリガーのリセットを行う
	// 複数落下アニメーション再生中トリガー
	isMultiFalling_ = false;
	// 複数落下時のアニメーション終了トリガー
	isMultiFallEnd_ = true;
	// 複数落下アニメーション中の攻撃アニメーショントリガー
	playMultiFallAttackAnim_ = false;
}

void BossAnimManager::PlayFallAttackAnim(float readyTime)
{
	// アニメーションのループ無効
	anim_->isLoop_ = false;

	// アニメーションの読み込みパラメータ変更
	anim_->ChangeParameter("Boss_StartFallAttack", true);

	// 準備時間の取得
	fallAttackReadyTime_ = readyTime;
	// 準備時間に基づいてタイマー開始
	animTimer_.Start(fallAttackReadyTime_);
}

void BossAnimManager::PlayPushUpAttackAnim(float readyTime)
{
	// アニメーションのループ無効
	anim_->isLoop_ = false;

	// アニメーションの読み込みパラメータ変更
	anim_->ChangeParameter("Boss_StartThrustUpAttack", true);

	// 準備時間の取得
	pushUpReadyTime_ = readyTime;
	// 準備時間に基づいてタイマー開始
	animTimer_.Start(pushUpReadyTime_);
}

void BossAnimManager::PlayMultiFallAnim()
{
	// アニメーションのループ無効
	anim_->isLoop_ = false;

	// 複数落下アニメーション再生中
	isMultiFalling_ = true;
	// 終了トリガーfalse
	isMultiFallEnd_ = false;
	// 攻撃アニメーショントリガーfalse
	playMultiFallAttackAnim_ = false;

	// アニメーションの読み込みパラメータ変更
	anim_->ChangeParameter("Boss_StartFallAttack", true);
}

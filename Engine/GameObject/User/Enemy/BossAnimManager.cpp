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

}

void BossAnimManager::Update()
{
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
	}

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

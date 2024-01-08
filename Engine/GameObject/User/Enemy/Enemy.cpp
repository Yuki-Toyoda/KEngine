#include "Enemy.h"
#include "EnemyBullet.h"
#include "../../GameObjectManager.h"

void Enemy::Init()
{
	// アニメーションマネージャの取得
	animManager_ = AnimationManager::GetInstance();

	// 各種ワールドトランスフォームの初期化
	bodyTransform_.Init();
	// 親子付け
	bodyTransform_.SetParent(&transform_, 0b011);
	headTransform_.Init();
	headTransform_.SetParent(&bodyTransform_, 0b111);
	headTransform_.translate_ = { 0.0f, 1.1f, 0.0f };
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_, 0b111);
	armTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_, 0b111);
	armTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };

	// アニメーションパラメータの生成

	// 待機状態
	CreateParameter("Enemy_Idle");
	// 弾発射までのチャージアニメーション
	CreateParameter("Enemy_Charge");
	// 弾発射までのチャージ中
	CreateParameter("Enemy_Charging");
	// 弾発射のアニメーション
	CreateParameter("Enemy_Shot");
	// 弾の跳ね返しアニメーション
	CreateParameter("Enemy_Rally");

	// アニメーションの作成
	enemyAnim_ = AnimationManager::GetInstance()->CreateAnimation("EnemyAnimation", "Enemy_Idle");
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Scale", &headTransform_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Rotate", &headTransform_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Translate", &headTransform_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Scale", &armTransform_R_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Rotate", &armTransform_R_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Translate", &armTransform_R_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Scale", &armTransform_L_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Rotate", &armTransform_L_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Translate", &armTransform_L_.translate_);

	// ループ状態にする
	enemyAnim_->isLoop_ = true;
	// この状態で再生
	enemyAnim_->Play();

	// メッシュを追加
	AddMesh(&bodyTransform_, color_, "./Resources/Enemy", "Body.obj");
	AddMesh(&headTransform_, color_, "./Resources/Enemy", "Head.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Enemy", "Arm_R.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Enemy", "Arm_L.obj");

	// 球のコライダー追加
	AddColliderSphere("Boss", &worldPos_, &colliderRadius_);

	// 行動変更
	ChangeState(std::make_unique<EnemyRoot>());
}

void Enemy::Update()
{
	// 差分ベクトルを求める
	Vector3 sub = transform_.translate_ - playerPos_->translate_;
	//sub = Math::Normalize(sub);

	// 目標角度の取得
	targetAngle_ = std::atan2(sub.x, sub.z);

	// 目標角度に向かって徐々に補間
	transform_.rotate_.y = Math::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);

	// 現在行動を更新
	state_->Update();

	// ワールド座標の取得
	worldPos_ = transform_.GetWorldPos();
}

void Enemy::DisplayImGui()
{
	transform_.DisplayImGui();

	bodyTransform_.DisplayImGuiWithTreeNode("BodyTransform");
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R_Transform");
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L_Transform");

	enemyAnim_->DisplayImGui();

	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("ChangeReadParameter")) {
		if (ImGui::Button("Idle")) {
			enemyAnim_->ChangeParameter("Enemy_Idle", true);
		}
		if (ImGui::Button("Charge")) {
			enemyAnim_->ChangeParameter("Enemy_Charge", true);
		}
		if (ImGui::Button("Charging")) {
			enemyAnim_->ChangeParameter("Enemy_Charging", true);
		}
		if (ImGui::Button("Shot")) {
			enemyAnim_->ChangeParameter("Enemy_Shot", true);
		}
		if (ImGui::Button("Rally")) {
			enemyAnim_->ChangeParameter("Enemy_Rally", true);
		}
		ImGui::TreePop();
	}

	// 発射時の行動でない場合
	if (state_->GetStateName() != "Shot") {
		// ボタンを押したら行動を設定
		if (ImGui::Button("PlayShot")) {
			ChangeState(std::make_unique<EnemyShot>());
		}
	}
}

void Enemy::OnCollisionEnter(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	}

	// 弾と衝突していたら
	if (collider->GetColliderName() == "Bullet") {
		EnemyBullet* b = GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet");
		if (b->GetIsReturn()) {
			// アニメーションを変更
			// ループを切る
			enemyAnim_->isLoop_ = false;
			enemyAnim_->ChangeParameter("Enemy_Rally", true);
		}
	}
}

void Enemy::OnCollision(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		//color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	}
}

void Enemy::OnCollisionExit(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

void Enemy::CreateParameter(const std::string& name)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Translate");
}

void Enemy::ChangeState(std::unique_ptr<IEnemyState> newState)
{
	// 共通初期化を行う
	newState->PreInit(this);
	// 初期化を行う
	newState->Init();

	// 初期化した新しいステートを代入
	state_ = std::move(newState);
}

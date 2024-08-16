#include "Enemy.h"
#include "EnemyBullet.h"
#include "../Player/Player.h"
#include "../../GameObjectManager.h"
#include "../Player/Player.h"

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
	// ダウンモーション
	CreateParameter("Enemy_Down");
	// ダウン中モーション
	CreateParameter("Enemy_Downing");
	// ダメージアニメーション
	CreateParameter("Enemy_Damage");
	// 死亡アニメーション
	CreateParameter("Enemy_Dead");


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
	enemyAnim_->AddAnimationKeys<Vector3>("Translate", &transform_.translate_);
	enemyAnim_->AddAnimationKeys <float> ("Alpha", &color_.w);

	// ループ状態にする
	enemyAnim_->isLoop_ = true;
	// この状態で再生
	enemyAnim_->Play();

	// メッシュを追加
	AddNormalModel(&bodyTransform_, "./Resources/Enemy", "Body.obj");
	AddNormalModel(&headTransform_, "./Resources/Enemy", "Head.obj");
	AddNormalModel(&armTransform_R_, "./Resources/Enemy", "Arm_R.obj");
	AddNormalModel(&armTransform_L_, "./Resources/Enemy", "Arm_L.obj");

	// 球のコライダー追加
	AddColliderSphere("Boss", &worldPos_, &colliderRadius_);

	// 行動変更
	ChangeState(std::make_unique<EnemyRoot>());
	// 行動変更タイマーリセット
	stateChangeTimer_.Start(kStateChangeCoolTime_);

	// 効果音読み込み
	damageSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwordDamage.mp3");
}

void Enemy::Update()
{
	// 死亡していなければ
	if (hp_ > 0) {
		// ダウン状態でない限り
		if (state_->GetStateName() != "Down") {
			// 差分ベクトルを求める
			Vector3 sub = transform_.translate_ - playerPos_->translate_;
			//sub = Math::Normalize(sub);

			// 目標角度の取得
			targetAngle_ = std::atan2(sub.x, sub.z);

			// 目標角度に向かって徐々に補間
			transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);
		}

		// 行動変更タイマーが終了していれば
		if (stateChangeTimer_.GetIsFinish()) {
			// ダウン状態でない限り変更を行わない
			if (state_->GetStateName() != "Down" && state_->GetStateName() != "Shot") {
				// y座標が特定の値に達してれば
				if (transform_.translate_.y >= 3.9f) {
					if (GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr) {
						// 敵が弾を撃つ
						ChangeState(std::make_unique<EnemyShot>());
						rallyCount_ = 0;
						// 行動変更タイマーリセット
						stateChangeTimer_.Start(kStateChangeCoolTime_);
					}
				}
			}
		}
	}

	// 現在行動を更新
	if (isGameStart_) {
		state_->Update();
	}

	// 死亡していなければ
	if (hp_ > 0) {
		if (isGameStart_) {
			// ヒットクールタイムタイマー更新
			hitCoolTimeTimer_.Update();
			// 行動変更クールタイムタイマー更新
			if (state_->GetStateName() == "Root"
				&& GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr
				&& GameObjectManager::GetInstance()->GetGameObject<Player>("Player")->GetStateName() != "Damage") {
				stateChangeTimer_.Update();
			}

			// 行動変更クールタイムタイマー更新
			if (enemyAnim_->GetReadingParameterName() == "Enemy_Rally") {
				if (enemyAnim_->isLoop_) {
					enemyAnim_->isLoop_ = false;
				}

				if (enemyAnim_->isEnd_) {
					// 行動変更
					ChangeState(std::make_unique<EnemyRoot>());
				}
			}
		}
	}
	else {
		if (state_->GetStateName() != "Dead") {
			ChangeState(std::make_unique<EnemyDead>());
		}
	}
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

	ImGui::DragInt("HP", &hp_);

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
		if (ImGui::Button("Down")) {
			enemyAnim_->ChangeParameter("Enemy_Down", true);
		}
		if (ImGui::Button("Downing")) {
			enemyAnim_->ChangeParameter("Enemy_Downing", true);
		}
		if (ImGui::Button("Damage")) {
			enemyAnim_->ChangeParameter("Enemy_Damage", true);
		}
		if (ImGui::Button("Dead")) {
			enemyAnim_->ChangeParameter("Enemy_Dead", true);
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

	// 発射時の行動でない場合
	if (state_->GetStateName() != "Down") {
		// ボタンを押したら行動を設定
		if (ImGui::Button("PlayDown")) {
			ChangeState(std::make_unique<EnemyDown>());
		}
	}

	// 発射時の行動でない場合
	if (state_->GetStateName() != "Dead") {
		// ボタンを押したら行動を設定
		if (ImGui::Button("PlayDead")) {
			ChangeState(std::make_unique<EnemyDead>());
		}
	}
}

void Enemy::OnCollisionEnter(Collider* collider)
{
	// 死亡していなければ
	if (hp_ > 0) {

		// 弾と衝突していたら
		if (collider->GetColliderName() == "Bullet") {
			EnemyBullet* b = GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet");
			if (b->GetIsReturn()) {
				// ラリー回数が最大数を超えていなければ
				if (rallyCount_ <= kMaxRallyCount_) {
					int percent = KLib::Random(rallyCount_, 6);
					if (rallyCount_ == 1) {
						percent = 0;
					}

					if (percent == 6) {
						// ダウン状態に
						ChangeState(std::make_unique<EnemyDown>());

						// 衝突した弾を破壊
						collider->GetGameObject()->Destroy();

						// ラリー回数リセット
						rallyCount_ = 0;
					}
					else {
						// アニメーションを変更
						// ループを切る
						enemyAnim_->isLoop_ = false;
						enemyAnim_->ChangeParameter("Enemy_Rally", true);
						// ラリー回数加算
						rallyCount_++;
					}

				}
				else {
					// ダウン状態に
					ChangeState(std::make_unique<EnemyDown>());

					// 衝突した弾を破壊
					collider->GetGameObject()->Destroy();

					// ラリー回数リセット
					rallyCount_ = 0;
				}
			}
		}
	}
}

void Enemy::OnCollision(Collider* collider)
{
	// 死亡していなければ
	if (hp_ > 0) {
		// 剣と衝突していたら
		if (collider->GetColliderName() == "Sword") {
			// ヒットクールタイムが終了していれば
			if (hitCoolTimeTimer_.GetIsFinish() && state_->GetStateName() == "Down") {
				// HPを減らす
				hp_--;
				// クールタイムタイマー開始
				hitCoolTimeTimer_.Start(kHitCoolTime_);

				// ループを切る
				enemyAnim_->isLoop_ = false;
				enemyAnim_->ChangeParameter("Enemy_Damage", true);
				//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 5, 5, transform_.translate_, 0.1f, 0.15f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));

				// ダメージ効果音の再生
				Audio::GetInstance()->PlayWave(damageSound_);
			}
		}
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
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Translate");
	animManager_->AddSelectAnimationKeys<float>(name, "Alpha");

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

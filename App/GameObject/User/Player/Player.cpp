#include "Player.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "App/GameObject/User/Enemy/EnemyBullet.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"

void Player::Init()
{
	// 入力取得
	input_ = Input::GetInstance();

	//武器用のトランスフォームの初期化
	weaponTransform_.Init();
	weaponTransform_.SetParent(&transform_);

	// コライダー用のトランスフォームの初期化
	colliderTransform_.Init();
	colliderTransform_.SetParent(&transform_);
	colliderTransform_.translate_.y += 1.0f;

	// メッシュの追加を行う
	AddSkiningModel("Player", &transform_, "./Resources/Player", "Player.gltf");
	AddSkiningModel("Sword", &weaponTransform_, "./Resources/Sword", "Sword.gltf");

	// 待機アニメーションの再生
	skiningModels_["Player"]->animationManager_.PlayAnimation("00_Idle", 0.0f, true);
	// 武器を左手に追従するようにする
	skiningModels_["Player"]->SetBoneParent("WeaponAnchor", &weaponTransform_);

	// 剣の鏡面反射を有効にする
	skiningModels_["Sword"]->materials_[1].environmentCoefficient_ = 0.85f;
	// ブルームエフェクトを際立たせるためにライティング無効
	skiningModels_["Sword"]->materials_[1].enableLighting_ = false;

	// コライダーの追加
	colliderWorldPos_ = colliderTransform_.GetWorldPos();
	AddColliderSphere("PlayerCollider", &colliderWorldPos_, &colliderRadius_);

	// 攻撃用の線の追加
	attackLine_ = std::make_unique<Line>();
	attackLine_->Init("AttackLine", {0.0f, 0.0f, 0.0f}, {0.35f, 0.35f}, 1.0f);
	attackLine_->SetParent(&weaponTransform_);
	attackLine_->AddCollider("Sword", this);
	attackLine_->rotate_.z					= (float)std::numbers::pi;
	attackLine_->isActive_					= false;
	attackLine_->isDisplayTrail_			= true;
	attackLine_->trailMaterial_.tex_		= TextureManager::Load("./Engine/Resource/Samples/Texture", "SwordTrail.png");
	attackLine_->trailMaterial_.color_.w	= 0.0f;
	// 一度更新する
	attackLine_->Update();

	// 行動状態を待機状態に変更
	ChangeState(std::make_unique<Root>());

	// 帯スプライト
	AddSprite("UpperObi", {640.0f, 0.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	AddSprite("LowerObi", {640.0f, 720.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	// 生成したスプライトの生成
	sprites_["UpperObi"]->anchorPoint_ = { 0.5f, 0.0f };
	sprites_["UpperObi"]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	sprites_["LowerObi"]->anchorPoint_ = { 0.5f, 1.0f };
	sprites_["LowerObi"]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	// UIの追加
	for (int i = 0; i < 6; i++) {
		hertUISize_[i] = {48.0f, 48.0f };
		hertUITranslate_[i] = { (hertUISize_->x) + (hertUISize_->x * i) , 32.0f };
		// ハートスプライト名の取得
		std::string hert = "Hert" + std::to_string(i);
		AddSprite(hert, hertUITranslate_[i], hertUISize_[i], TextureManager::Load("./Resources", "Hert.png"));
		sprites_[hert]->color_ = { 1.0f, 0.0f, 0.15f, 1.0f };
		sprites_[hert]->isActive_ = false;
	}

	// 攻撃ボタン
	AddSprite("AButton", { 1125.0f, 125.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_A.png"));
	// 剣アイコン
	AddSprite("SwordIcon", { 1140.0f, 142.5f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Icon", "MasterSword.png"));
	// Bボタン
	AddSprite("BButton", { 1185.0f, 75.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_B.png"));

	// ボタン関連UI非表示
	sprites_["AButton"]->isActive_ = false;
	sprites_["SwordIcon"]->isActive_ = false;
	sprites_["BButton"]->isActive_ = false;

	// 効果音読み込み
	SwingSword_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwingSword.mp3");
	RotateSlash_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/RotateSlash.mp3");
}

void Player::Update()
{
	// 行動可能時
	if (canAction_) {
		// 現在の行動状態の更新を行う
		state_->Update();

		// 追従カメラがセットされている場合
		if (followCamera_ != nullptr) {
			// Z注目が有効にされている場合
			if (!followCamera_->GetEnableZForcus()) {
				followCamera_->SetTargetAngle(followCamera_->transform_.rotate_.y);
				// ロックオン中は徐々にビネット解除
				focusVignette_ = KLib::Lerp<float>(focusVignette_, 0.0f, 0.1f);
			}
			else {
				// ロックオン中は徐々にビネット
				focusVignette_ = KLib::Lerp<float>(focusVignette_, 1.0f, 0.1f);
			}

			// ビネットをかける
			followCamera_->ppProcessor_.vignette_.intensity_ = focusVignette_;

			if (followCamera_->GetLockOn()->GetIsLockOn()) {
				// ロックオン対象の座標
				Vector3 targetPos = followCamera_->GetLockOn()->target_->transform_.translate_;
				// 追従対象からロックオン対象への差分ベクトル
				Vector3 sub = targetPos - transform_.translate_;
				// 方向ベクトルを元にプレイヤーがいる角度を求める
				targetAngle_ = std::atan2(sub.x, sub.z);

				// 身体を回転させる
				transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);
			}

			// ブラーの値の参照取得
			float& blurStrength = followCamera_->ppProcessor_.radialBlur_.data_.blurStrength;
			// パリィブラー有効時
			if (enableParryBlur_) {
				// ブラー強さが一定の値を超えるまで
				if (blurStrength <= 0.01f) {
					// ブラー強さを少しずつ上げる
					blurStrength = KLib::Lerp(blurStrength, 0.0115f, 0.35f);
				}
				else {
					// パリィ時のブラーを無効
					enableParryBlur_ = false;
				}
			}
			else {
				// かかっているブラーを元に戻す
				if (blurStrength > 0.0f) {
					blurStrength = KLib::Lerp(blurStrength, 0.00f, 0.05f);
				}
			}
		}

		// 軌跡a値
		float& trailAlpha = attackLine_->trailMaterial_.color_.w;
		// 攻撃中は軌跡を表示させる
		if (isAttacking_) {
			trailAlpha = KLib::Lerp(trailAlpha, 1.0f, 0.15f);
		}
		else {
			// 攻撃中でない場合は軌跡を徐々に消す
			if (trailAlpha <= 0.01f) {
				trailAlpha = 0.0f;
			}
			else {
				trailAlpha = KLib::Lerp(trailAlpha, 0.0f, 0.2f);
			}
			
		}

		// 攻撃可能か
		if (state_->GetStateName() != "Damage") {
			if (canAttack_ && !isAttacking_) {
				// Aボタンを押すと攻撃する
				if (input_->InspectButton(XINPUT_GAMEPAD_A, TRIGGER)) {
					// 行動を変更
					ChangeState(std::make_unique<Attack>());
				}
			}
		}

		// 線の更新
		attackLine_->Update();

		//コライダーのワールド座標更新
		colliderWorldPos_ = colliderTransform_.GetWorldPos();

		// ヒットクールタイム更新
		hitCoolTimeTimer_.Update();
	}
	else {
		// ゲーム開始状態のとき
		if (gameManager_->GetIsGameStart() && !isSetUp_) {
			// メインカメラを使用
			followCamera_->UseThisCamera();

			// 体力UIの表示
			for (int i = 0; i < 6; i++) {
				std::string hert = "Hert" + std::to_string(i);
				sprites_[hert]->isActive_ = true;
			}
			// ボタン関連UIの表示
			sprites_["AButton"]->isActive_		= true;
			sprites_["SwordIcon"]->isActive_	= true;
			sprites_["BButton"]->isActive_		= true;

			// 行動可能状態に
			canAction_ = true;

			// セットアップした状態に
			isSetUp_ = true;
		}
	}

	// 敵弾が存在する場合帯を出現させる
	if (GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") != nullptr) {
		sprites_["UpperObi"]->scale_.y = KLib::Lerp<float>(sprites_["UpperObi"]->scale_.y, 75.0f, 0.1f);
		sprites_["LowerObi"]->scale_.y = KLib::Lerp<float>(sprites_["LowerObi"]->scale_.y, 75.0f, 0.1f);
	}
	else {
		sprites_["UpperObi"]->scale_.y = KLib::Lerp<float>(sprites_["UpperObi"]->scale_.y, 0.0f, 0.2f);
		sprites_["LowerObi"]->scale_.y = KLib::Lerp<float>(sprites_["LowerObi"]->scale_.y, 0.0f, 0.2f);
	}
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();

	ImGui::DragInt("HP", &hp_);

	// 行動状態のImGuiを表示
	state_->DisplayImGui();

	// 線のImGui表示
	attackLine_->DisplayImGui();

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();
}

void Player::ChangeState(std::unique_ptr<IState> newState)
{
	// 共通初期化を行う
	newState->PreInit(this);
	// 初期化を行う
	newState->Init();

	// 初期化した新しいステートを代入
	state_ = std::move(newState);
}

void Player::HitDamage(const Vector3& translate)
{
	// ヒットクールタイムが終了していれば
	if (hitCoolTimeTimer_.GetIsFinish()) {
		// HPを減らす
		hp_--;

		// スプライト配列探査用
		std::string hert = "Hert" + std::to_string(hp_);
		// スプライトの色を変更
		sprites_[hert]->color_ = { 0.15f, 0.15f, 0.15f, 1.0f };

		// 弾の座標からプレイヤーの方向ベクトルを求める
		Vector3 sub = translate - transform_.translate_;
		// 角度を求める
		transform_.rotate_.y = std::atan2(sub.x, sub.z);

		// ダメージステートに移行
		ChangeState(std::make_unique<Damage>());

		// ロックオン無効
		lockOn_->DisableLockOn();

		// 命中クールタイムリセット
		hitCoolTimeTimer_.Start(kHitCoolTime_);

		// ダメージ時にはラジアルブラー無効
		enableParryBlur_ = false;
	}
}

void Player::OnCollisionEnter(Collider* collider)
{
	// プレイヤーの剣と衝突していたら
	if (collider->GetColliderName() == "Bullet" && !enableParryBlur_ && isAttacking_) {
		// パリィ時のブラーを有効
		enableParryBlur_ = true;
	}
}

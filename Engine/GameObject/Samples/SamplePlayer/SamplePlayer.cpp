#include "SamplePlayer.h"
#include "../SampleThirdPersonCamera/ThirdPersonCamera.h"
#include "../SampleWeapon/SampleWeapon.h"
#include "../SampleEnemy/SampleEnemy.h"
#include "../SampleLockOn/LockOn.h"
#include "../../GameObjectManager.h"

void SamplePlayer::Initialize()
{
	// ワールド座標初期化
	bodyTransform_.Initialize(); // 体
	bodyTransform_.translate_ = { 0.0f, 0.0f, 0.0f }; // 初期座標設定
	bodyTransform_.SetParent(&transform_); // 親子付け
	headTransform_.Initialize(); // 頭
	headTransform_.translate_ = { 0.0f, 1.5f, 0.0f }; // 初期座標設定
	headTransform_.SetParent(&bodyTransform_); // 親子付け
	armTransform_L_.Initialize(); // 左腕
	armTransform_L_.translate_ = { -0.35f, 1.25f, 0.0f }; // 初期座標設定
	armTransform_L_.SetParent(&bodyTransform_); // 親子付け
	armTransform_R_.Initialize(); // 右腕
	armTransform_R_.translate_ = { 0.35f, 1.25f, 0.0f }; // 初期座標設定
	armTransform_R_.SetParent(&bodyTransform_); // 親子付け

	// モデル読み込み
	AddMesh(&bodyTransform_, color_, "./Engine/Resource/Samples/Player/Body", "float_Body.obj");
	AddMesh(&headTransform_, color_, "./Engine/Resource/Samples/Player/Head", "float_Head.obj");
	AddMesh(&armTransform_L_, color_, "./Engine/Resource/Samples/Player/L_Arm", "float_L_arm.obj");
	AddMesh(&armTransform_R_, color_, "./Engine/Resource/Samples/Player/R_Arm", "float_R_arm.obj");

	// 入力状態取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
	// 腕振りギミック初期化
	InitializeArmSwingGimmick();

	// リスタートフラグ
	restart_ = false;

	// 目標角度リセット
	targetAngle_ = Math::MakeIdentity4x4();

	// 接地判定
	isLanding_ = false;
	// 最大落下速度
	kMaxFallSpeed_ = -0.98f;
	// 現在落下速度
	fallSpeed_ = 0.0f;
	// 落下加速度
	kFallAcceleration_ = 0.0098f;

	// ジャンプ不可能に
	canJump_ = false;
	// ジャンプ速度初期化
	jumpSpeed_ = 0.0f;
	// 最大ジャンプ速度設定
	kMaxJumpHeight_ = 1.25f;
	// ジャンプ減衰速度を設定
	kJumpDecayRate_ = 0.098f;

	// 腕振りサイクル
	armSwingCycle_ = 60;
	// 腕振りギミック用変数
	armSwingParameter_ = 0.0f;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

}

void SamplePlayer::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// ふるまいを変更するリクエストがあればTrue
	if (behaviorRequest_) {
		// ふるまい変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
			// 通常行動初期化
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			// 攻撃行動初期化
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			// 攻撃行動初期化
			BehaviorDashInitialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いごとの初期化を実行
	switch (behavior_) {
	case Behavior::kRoot:
		// 通常行動初期化
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		// 攻撃行動初期化
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		// 攻撃行動初期化
		BehaviorDashUpdate();
		break;
	}

	// 接地していないなら
	if (!isLanding_) {

		if (transform_.GetParent() != nullptr) {
			transform_.translate_ = transform_.GetWorldPos();
			transform_.SetParent(nullptr);
		}
		// 最大落下速度を超過するまで
		if (fallSpeed_ >= kMaxFallSpeed_) {
			// 落下速度加算
			fallSpeed_ -= kFallAcceleration_;
		}
		else {
			// 超過していた場合は落下速度を最大速度に設定
			fallSpeed_ = kMaxFallSpeed_;
		}
	}
	else {
		// 接地しているなら落下速度初期化
		fallSpeed_ = 0.0f;
	}

	// 落下スピード加算
	transform_.translate_.y += fallSpeed_;

	// 落下したら初期位置に戻す
	if (transform_.translate_.y <= -100.0f) {
		fallSpeed_ = 0.0f;
		transform_.translate_ = { 0.0f, 5.0f, 0.0f };
		transform_.rotate_ = { 0.0f, 0.0f, 0.0f };
	}

	// 接地していないなら
	if (!isLanding_) {
		// ジャンプ処理
		transform_.translate_.y += jumpSpeed_;
	}

	// ジャンプ速度がでないなら
	if (jumpSpeed_ >= 0.0f) {
		// ジャンプ速度減衰
		jumpSpeed_ -= kJumpDecayRate_;
	}
	else {
		// ジャンプ速度を0に
		jumpSpeed_ = 0.0f;
	}

	// 敵の取得
	enemies_.clear();
	enemies_ = GameObjectManager::GetInstance()->GetGameObjectList<SampleEnemy>();

	// ロックオンしている敵リストにセット
	lockOn_->SetEnemyList(enemies_);

	// 着地判定リセット
	isLanding_ = false;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}

	tpCamera_->UpdateTarget();
}

void SamplePlayer::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui();
}

void SamplePlayer::OnCollisionEnter(BaseObject* object)
{
	if (object->GetObjectTag() == TagFloor) {

		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		// 接地判定On
		isLanding_ = true;

		// 追従カメラがセットされていたら
		if (tpCamera_ != nullptr) {
			// 追従座標更新
			tpCamera_->UpdateTarget();
		}
	}

	if (object->GetObjectTag() == TagEnemy) {
		// ダッシュ中ならリスタート
		if (behavior_ == kDash)
			restart_ = true;
	}
}

void SamplePlayer::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case TagEnemy: // 衝突したオブジェクトが敵であった場合

		break;
	case TagFloor: // 衝突したオブジェクトが床の場合
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		// 接地判定On
		isLanding_ = true;
		break;
	}
}

void SamplePlayer::OnCollisionExit(BaseObject* object)
{
	object;
}

void SamplePlayer::SetWeapon(SampleWeapon* weapon)
{
	// インスタンス取得
	weapon_ = weapon;
	// プレイヤー座標を親とする
	weapon_->transform_.SetParent(&armTransform_R_, 0b111);
}

void SamplePlayer::BehaviorRootInitialize()
{
	// 浮遊ギミック初期化
	InitializeFloatingGimmick();
	// 腕振りギミック初期化
	InitializeArmSwingGimmick();
}

void SamplePlayer::BehaviorRootUpdate()
{
	// 移動速度
	const float speed = 0.3f;
	// デッドゾーン
	const float deadZone = 0.7f;
	// 移動フラグ
	bool isMoving = false;

	// スティックの入力に応じて移動
	Vector3 move = {
		(float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		(float)joyState_.Gamepad.sThumbLY / SHRT_MAX };
	if (Math::Length(move) > deadZone)
		isMoving = true;

	if (isMoving) {
		// 移動量を正規化、スピードを加算
		move = Math::Normalize(move);
		float moveDot = 0.0f;

		// カメラ回転角がセットされている場合
		if (tpCamera_ != nullptr) {
			// カメラの角度から回転行列を生成
			Vector3 v = { 0.0f, 0.0f, 1.0f };
			Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(tpCamera_->transform_.rotate_);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = Math::Transform(move, rotateMat);
			moveDot = Math::Dot(v, move);
			targetAngle_ = Math::DirectionToDirection({0.0f, 0.0f, 1.0f}, move);
			if (moveDot == -1.0f)
				targetAngle_ = targetAngle_ + Math::MakeRotateYMatrix((float)std::numbers::pi);
			move = Math::Normalize(move) * speed;
		}

		// 移動
		transform_.translate_ = transform_.translate_ + move;

		// トランスフォームに回転行列をセット
		transform_.SetRotateMat(targetAngle_);
	}

	// ジャンプ可能なら
	if (canJump_) {
		// Aボタンが押されたら
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
			!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// ジャンプさせる
			jumpSpeed_ = kMaxJumpHeight_;
			// ジャンプ不可に
			canJump_ = false;
			// 接地していない状態に
			isLanding_ = false;
		}
	}
	else {
		// 接地しているなら
		if (isLanding_) {
			// ジャンプ可能に
			canJump_ = true;
		}
	}

	// 武器座標をありえないほど遠くに
	if (weapon_ != nullptr)
		weapon_->transform_.translate_ = { -100000.0f, -100000.0f, 0.0f };

	// リクエスト状態が攻撃行動でない、接地状態であれば
	if (behaviorRequest_ != kAttack && isLanding_) {
		// Ｘボタンが入力されたら
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			// 次の行動を攻撃行動に
			behaviorRequest_ = kAttack;
		}
	}

	// リクエスト状態がダッシュ行動でない、接地状態であれば
	if (behaviorRequest_ != kDash && behavior_ != kAttack) {
		// Ｘボタンが入力されたら
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) &&
			!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			// 次の行動を攻撃行動に
			behaviorRequest_ = kDash;
		}
	}

	// 浮遊ギミック更新
	UpdateFloatingGimmick();
	// 腕振りギミック更新
	UpdateArmSwingGimmick();
}

void SamplePlayer::BehaviorAttackInitialize()
{
	// 攻撃状態初期化
	attackState_ = SwingOver;
	// t初期化
	t_ = 0.0f;

	// 座標設定
	if (weapon_ != nullptr) {
		weapon_->transform_.rotate_.x = (float)std::numbers::pi;
		weapon_->transform_.translate_ = { -0.35f, 0.15f, 0.0f };
	}
}

void SamplePlayer::BehaviorAttackUpdate()
{
	// 現在の攻撃状態によって遷移
	switch (attackState_) {
	case SamplePlayer::SwingOver:
		// イージングによって振りかぶる動作を行う
		if (t_ <= swingOverTime_) {
			// イージングで腕の角度を調整
			armTransform_R_.rotate_.x =
				Math::EaseOut(t_, swingOverStartAngle_, swingOverEndAngle_, swingOverTime_);
			armTransform_L_.rotate_.x =
				Math::EaseOut(t_, swingOverStartAngle_, swingOverEndAngle_, swingOverTime_);
			// t加算
			t_ += 1.0f / 60.0f;
		}
		else {
			// 演出用tリセット
			t_ = 0.0f;

			// 攻撃開始地点を設定
			attackStartPos_ = transform_.translate_;
			// 攻撃終端地点を設定
			// スティックの入力に応じて移動
			Vector3 move = {
				0.0f, 0.0f, 1.0f };

			// 移動量を正規化、スピードを加算
			move = Math::Normalize(move) * attackForward_;

			// カメラの角度から回転行列を生成
			//Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = Math::Transform(move, targetAngle_);

			// 攻撃終端地点を設定
			attackEndPos_ = attackStartPos_ + move;

			// 次の段階へ
			attackState_++;
		}
		break;
	case SamplePlayer::Attack:
		// イージングによって攻撃動作を行う
		if (t_ <= attackTime_) {
			// イージングで腕の角度を調整
			armTransform_R_.rotate_.x =
				Math::EaseOut(t_, swingOverEndAngle_, attackEndAngle_, attackTime_);
			armTransform_L_.rotate_.x =
				Math::EaseOut(t_, swingOverEndAngle_, attackEndAngle_, attackTime_);

			// イージングでプレイヤーを移動
			transform_.translate_ =
				Math::EaseOut(t_, attackStartPos_, attackEndPos_, attackTime_);

			// 攻撃状態に
			weapon_->SetIsAttack(true);

			// t加算
			t_ += 1.0f / 60.0f;
		}
		else {
			// 演出用tリセット
			t_ = 0.0f;
			// 次の段階へ
			attackState_++;
		}
		break;
	case SamplePlayer::Wait:
		// 待機
		if (t_ <= attackWaitTime_) {

			// 攻撃していない状態に
			weapon_->SetIsAttack(false);

			// t加算
			t_ += 1.0f / 60.0f;
		}
		else {
			// 通常行動に戻すように命令
			behaviorRequest_ = kRoot;
		}
		// 演出終了
		break;
	default:
		break;
	}
}

void SamplePlayer::BehaviorDashInitialize()
{
	// ダッシュ変数初期化
	workDash_.dashParameter_ = 0; // 媒介変数
	workDash_.dashSpeed_ = 1.0f; // ダッシュスピード
	// プレイヤーを目標角度まで一気に補正
	//transform_.rotate_.y = targetAngle_;

	// 腕をナルト走りにする
	armTransform_L_.rotate_.x = (float)std::numbers::pi / 3.0f; // 角度設定
	armTransform_R_.rotate_.x = (float)std::numbers::pi / 3.0f; // 角度設定
}

void SamplePlayer::BehaviorDashUpdate()
{
	// ベクトル設定
	Vector3 move = {
		0.0f, 0.0f, workDash_.dashSpeed_ };

	// カメラの角度から回転行列を生成
	//Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);
	// 移動ベクトルをカメラの角度に応じて回転させる
	move = Math::Transform(move, targetAngle_);

	// 移動
	transform_.translate_ = transform_.translate_ + move;

	// ダッシュの時間
	const uint32_t behaviorDashTime = 15;

	// 既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime)
		behaviorRequest_ = Behavior::kRoot;
}

void SamplePlayer::InitializeFloatingGimmick()
{
	// 浮遊移動サイクル
	floatingCycle_ = 60;
	// 浮遊の振幅
	floatingAmpritude_ = 0.01f;
	// 変数初期化
	floatingParameter_ = 0.0f;
}

void SamplePlayer::UpdateFloatingGimmick()
{
	// 1フレームごとの加算値
	const float step = (float)(2.0f * std::numbers::pi / floatingCycle_);

	// パラメータを1ステップ分加算する
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = (float)(std::fmod(floatingParameter_, 2.0f * std::numbers::pi));

	// 浮遊を座標に反映させる
	bodyTransform_.translate_.y = std::sin(floatingParameter_) * floatingAmpritude_;
}

void SamplePlayer::InitializeArmSwingGimmick()
{
	// 腕振りサイクル
	armSwingCycle_ = 60;
	// 腕振りギミック用変数
	armSwingParameter_ = 0.0f;
}

void SamplePlayer::UpdateArmSwingGimmick()
{
	// 1フレームごとの加算値
	const float step = (float)(2.0f * std::numbers::pi / armSwingCycle_);

	// パラメータを1ステップ分加算する
	armSwingParameter_ += step;
	// 2πを超えたら0に戻す
	armSwingParameter_ = (float)(std::fmod(armSwingParameter_, 2.0f * std::numbers::pi));

	// 腕振りを座標に反映させる
	armTransform_L_.rotate_.x = std::cos(armSwingParameter_) / 2.0f;
	armTransform_R_.rotate_.x = std::cos(armSwingParameter_) / 2.0f;
}

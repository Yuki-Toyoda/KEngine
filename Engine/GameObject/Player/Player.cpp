#include "Player.h"
#include "../Camera/TPCamera.h"

void MyPlayer::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

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
	objects_.push_back(OBJ::Create(&bodyTransform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_Body.obj"));
	objects_.push_back(OBJ::Create(&headTransform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_Head.obj"));
	objects_.push_back(OBJ::Create(&armTransform_L_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_L_arm.obj"));
	objects_.push_back( OBJ::Create(&armTransform_R_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_R_arm.obj"));

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

	// 目標角度リセット
	targetAngle_ = 0.0f;

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

	onCollision_ = false;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void MyPlayer::Update()
{
	// 基底クラス更新
	BaseObject::Update();
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

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
		move = Math::Normalize(move) * speed;

		// カメラ回転角がセットされている場合
		if (tpCamera_ != nullptr) {
			// カメラの角度から回転行列を生成
			Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(tpCamera_->transform_.rotate_);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = Math::Transform(move, rotateMat);
		}

		// 移動
		transform_.translate_ = transform_.translate_ + move;

		// 移動時の目標角度を求める
		targetAngle_ = atan2(move.x, move.z);
	}

	// 角度を補正する
	transform_.rotate_.y =
		Math::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);

	// 浮遊ギミック更新
	UpdateFloatingGimmick();
	// 腕振りギミック更新
	UpdateArmSwingGimmick();

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

	// 着地判定リセット
	isLanding_ = false;
	onCollision_ = false;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), {1.0f, 1.0f, 1.0f});
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void MyPlayer::Draw()
{
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void MyPlayer::AddGlobalVariables()
{

}

void MyPlayer::ApplyGlobalVariables()
{

}

void MyPlayer::OnCollisionEnter(BaseObject* object)
{
	if (object->GetObjectTag() == Floor || object->GetObjectTag() == MoveFloor) {

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
}

void MyPlayer::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Enemy: // 衝突したオブジェクトが敵であった場合

		break;
	case Floor: // 衝突したオブジェクトが床の場合
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		// 接地判定On
		isLanding_ = true;
		break;
	case MoveFloor: // 衝突したオブジェクトが動く床の場合

		// とりあえず床の高さに補正
		// 接地判定On
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		isLanding_ = true;
		onCollision_ = true;
		if (transform_.GetParent() == nullptr) {

			Vector3 myWorldPos = transform_.GetWorldPos();
			Vector3 objectWorldPos = object->transform_.GetWorldPos();

			Vector3 position = myWorldPos - objectWorldPos;

			// 親の角度から回転行列を計算
			Matrix4x4 rotateMatrix = Math::MakeRotateXYZMatrix({ -object->transform_.rotate_.x , -object->transform_.rotate_.y,-object->transform_.rotate_.z });
			position = Math::TransformNormal(position, rotateMatrix);

			transform_.translate_ = position;
			transform_.SetParent(&object->transform_, 0b011);
		}

	}
}

void MyPlayer::OnCollisionExit(BaseObject* object)
{
	// 動く床から離れたなら
	if (object->GetObjectTag() == MoveFloor) {
		if (transform_.GetParent() != nullptr) {
			transform_.translate_ = transform_.GetWorldPos();
			transform_.SetParent(nullptr);
		}
	}
}

void MyPlayer::InitializeFloatingGimmick()
{
	// 浮遊移動サイクル
	floatingCycle_ = 60;
	// 浮遊の振幅
	floatingAmpritude_ = 0.01f;
	// 変数初期化
	floatingParameter_ = 0.0f;
}

void MyPlayer::UpdateFloatingGimmick()
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

void MyPlayer::InitializeArmSwingGimmick()
{
	// 腕振りサイクル
	armSwingCycle_ = 60;
	// 腕振りギミック用変数
	armSwingParameter_ = 0.0f;
}

void MyPlayer::UpdateArmSwingGimmick()
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

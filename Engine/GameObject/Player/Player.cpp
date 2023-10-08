#include "Player.h"

void MyPlayer::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create({ 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_Body.obj"));
	objects_.push_back(OBJ::Create({ 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_Head.obj"));
	objects_.push_back(OBJ::Create({ 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_L_arm.obj"));
	objects_.push_back(OBJ::Create({ 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "float_R_arm.obj"));

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

	// 腕振りサイクル
	armSwingCycle_ = 60;
	// 腕振りギミック用変数
	armSwingParameter_ = 0.0f;

	// 衝突属性を設定
	collider_->SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	collider_->SetCollisionMask(0x00000001);

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
		if (cameraRotation_ != nullptr) {
			// カメラの角度から回転行列を生成
			Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(*cameraRotation_);
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
	if (isActive_) {
		objects_[0]->SetWorldTransform(bodyTransform_);
		objects_[0]->Draw();
		objects_[1]->SetWorldTransform(headTransform_);
		objects_[1]->Draw();
		objects_[2]->SetWorldTransform(armTransform_L_);
		objects_[2]->Draw();
		objects_[3]->SetWorldTransform(armTransform_R_);
		objects_[3]->Draw();

	}
}

void MyPlayer::AddGlobalVariables()
{

}

void MyPlayer::ApplyGlobalVariables()
{

}

void MyPlayer::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Camera:
		break;
	case Enemy:
		break;
	case Other:
		break;
	default:
		break;
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

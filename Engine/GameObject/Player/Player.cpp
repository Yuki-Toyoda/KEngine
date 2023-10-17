#include "Player.h"

Player::Player() : gearTheta_(gearTransform_.rotate_.z)
{

}
Player::~Player()
{

}

void Player::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 入力状態取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 用意した変数の初期化
	InitializeVariables();

	AddOBJ(&gearTransform_, gearColor_, "./Resources/Gear", "Gear_l.obj");
	AddOBJ(&transform_, color_, "./Resources/Player", "Player.obj");

	// 衝突判定を取る図形を設定する
	Sphere* sphere = new Sphere();
	sphere->Initialize(transform_.GetWorldPos(), radius_);
	collider_->SetColliderShape(sphere);
	collider_->SetGameObject(this);


	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
	ApplyGlobalVariables();
}

void Player::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 操作受け付け
	GetOperation();


	// プレイヤーの更新
	UpdatePlayer();
	// 歯車の更新
	UpdateGear();

	// 回転中の位置の補正
	UpdatePlayerRotate();

	DebugGui();

	// 破壊されていない時
	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), radius_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void Player::Draw()
{
	DrawAllOBJ();
}

void Player::AddGlobalVariables()
{
	globalVariables_->AddItem("Gear", "kMaxGearRotateSpeed", kMaxGearRotateSpeed_);
	globalVariables_->AddItem("Gear", "kMinGearRollSpeed", kMinGearRollSpeed_);
	globalVariables_->AddItem("Gear", "kGearFriction", kGearFriction_);
	globalVariables_->AddItem("Gear", "kGearInnerRadius", kGearInnerRadius_);
	globalVariables_->AddItem("Gear", "kGearAmplitude", kGearAmplitude);
	globalVariables_->AddItem("Gear", "kGearDecreaseRate", kGearDecreaseRate_);
	globalVariables_->AddItem("Gear", "GearScale", Vector2(gearTransform_.scale_.x, gearTransform_.scale_.z));
	globalVariables_->AddItem("Player", "kMaxPlayerVelocity", kMaxPlayerVelocity_);
	globalVariables_->AddItem("Player", "kGravity", kGravity_);
	globalVariables_->AddItem("Player", "kPlayerJumpPower", kPlayerJumpPower_);
}

void Player::ApplyGlobalVariables()
{
	kMaxGearRotateSpeed_ = globalVariables_->GetFloatValue("Gear", "kMaxGearRotateSpeed");
	//kMinGearRollSpeed_ = globalVariables_->GetFloatValue("Gear", "kMinGearRollSpeed");
	//kGearFriction_ = globalVariables_->GetFloatValue("Gear", "kGearFriction");
	kGearInnerRadius_ = globalVariables_->GetFloatValue("Gear", "kGearInnerRadius");
	kGearAmplitude = globalVariables_->GetFloatValue("Gear", "kGearAmplitude");
	kGearDecreaseRate_ = globalVariables_->GetFloatValue("Gear", "kGearDecreaseRate");
	Vector2 temp = globalVariables_->GetVector2Value("Gear", "GearScale");
	gearTransform_.scale_ = Vector3(temp.x, temp.x, temp.y);
	kMaxPlayerVelocity_ = globalVariables_->GetFloatValue("Player", "kMaxPlayerVelocity");
	//kGravity_ = globalVariables_->GetFloatValue("Player", "kGravity");
	kPlayerJumpPower_ = globalVariables_->GetFloatValue("Player", "kPlayerJumpPower");
}

void Player::OnCollisionEnter(BaseObject* object)
{
	object;
}

void Player::OnCollision(BaseObject* object)
{
	object;
	// 衝突しているのが Item だった時
	if (object->GetObjectTag() == tagItem) {
		// ボタンの再入力があった時
		if (input_->PushKey(DIK_Q)) {
			AirJump();
		}
	}
}

void Player::OnCollisionExit(BaseObject* object)
{
	object;
}

/// プライべート関数

void Player::InitializeVariables()
{

	// プレイヤー変数初期化
	playerVelocity_ = { 0.0f,0.0f,0.0f };
	playerAcceleration_ = { 0.0f,0.0f,0.0f };
	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	playerTheta_ = 0.0f;
	radius_ = 1.0f;
	transform_.scale_ = { 1.0f,1.0f,5.0f };

	// 歯車変数初期化
	gearTransform_.Initialize();
	gearTransform_.scale_ = { 8.0f,8.0f,5.0f };
	gearTheta_ = gearTransform_.rotate_.z;
	// 白色不透明
	gearColor_ = color_;
	gearRotateSpeed_ = 0.0f;

	// フラグ初期化
	isJumpTrigger_ = false;
	isRotateRight_ = true;
	isLanding_ = true;
	isPendulum_ = false;
	wasRotateRight_ = true;

	// 定数の再定義
	kGearInnerRadius_ = 0.5f;
	// さすがに半回転以上は行かない
	kMaxGearRotateSpeed_ = 0.1f;
	kMinGearRollSpeed_ = 0.005f;
	kGearFriction_ = 0.001f;
	kGearAmplitude = 1.4f;
	kGearDecreaseRate_ = 0.90f;
	kFallDirection_ = { 0.0f,-1.0f,0.0f };
	kPlayerJumpPower_ = 0.05f;
	kMaxPlayerVelocity_ = 1.0f;
	kGravity_ = 0.005f;
	// 定数として定義されているものは読み込む
	//ApplyGlobalVariables();
}

void Player::GetOperation()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	if (input_->TriggerKey(DIK_Q)) {
		if (isLanding_) {
			isJumpTrigger_ = true;
		}
	}
}

void Player::UpdatePlayer()
{
	// 歯車とくっついている時
	if (isLanding_) {
		// 角度からの回転方向
		CheckDirectionRotate();
		// ジャンプ入力があった場合
		if (isJumpTrigger_) {
			// ジャンプさせる
			playerVelocity_ = { -std::cosf(playerTheta_),-std::sinf(playerTheta_),0.0f };
			playerVelocity_.x *= kPlayerJumpPower_;
			playerVelocity_.y *= kPlayerJumpPower_;
			playerAcceleration_ = { 0.0f,0.0f,0.0f };
			isJumpTrigger_ = false;
			isLanding_ = false;
			isPendulum_ = false;
		}
		// 歯車の回転は歯車の更新内
	}
	else {
		// 重力減算
		Vector3 fallVelocity{};
		fallVelocity.x = kFallDirection_.x * kGravity_;
		fallVelocity.y = kFallDirection_.y * kGravity_;
		fallVelocity.z = 0.0f;
		playerAcceleration_.x = fallVelocity.x;
		playerAcceleration_.y = fallVelocity.y;
		playerVelocity_.x += playerAcceleration_.x;
		playerVelocity_.y += playerAcceleration_.y;
		// プレイヤーの位置を更新する
		transform_.translate_.x += playerVelocity_.x;
		transform_.translate_.y += playerVelocity_.y;

		// 歯車との当たり判定を取らせる
		CheckGearCollision();
	}
}

void Player::UpdateGear()
{
	// 抵抗で遅くする
	if (isLanding_) {
		// プレイヤーがくっついている時は振り子
		if (gearRotateSpeed_ != 0) {
			// float にキャストしたπを取得
			float pi = static_cast<float>(std::numbers::pi);

			// ラジアンは右から下への計算だと思うので、計算しやすくする
			// ラジアンの計算は右から上に増える
			float directionRotate = playerTheta_ - pi / 2.0f;
			// プレイヤーの回転角を制限する
			//playerTheta_ = std::fmodf(playerTheta_, pi);
			if (pi < directionRotate) {
				directionRotate = -pi + (pi - directionRotate);
			}
			else if (directionRotate < -pi) {
				directionRotate = pi + (pi + directionRotate);
			}
			// 振り子の運動をさせる
			if (kGearAmplitude < directionRotate) {
				//gearRotateSpeed_ -= kGearFriction_;
			}
			else if (directionRotate < -kGearAmplitude) {
				//gearRotateSpeed_ += kGearFriction_;
			}
			// 振り子範囲内の時
			if ((-pi - kGearAmplitude) / 2.0f < playerTheta_ &&
				playerTheta_ < (pi - kGearAmplitude) / 2.0f) {
				// 範囲内に入った瞬間の時振り子フラグ true
				if (!isPendulum_) {
					isPendulum_ = true;
					wasRotateRight_ = isRotateRight_;
				}
			}
			// 範囲外の時
			if (playerTheta_ < (-pi - kGearAmplitude) &&
				(pi - kGearAmplitude) < playerTheta_){
					// もし振り子挙動していたら、フラグ false
					if (isPendulum_) {
						isPendulum_ = false;
					}
			}
		}

		// プレイヤーがくっついている時
		//if (isLanding_) {
			// 速度を加算する
		// 振り子運動をさせる
		if (isPendulum_) {
			if (isRotateRight_) {
				gearRotateSpeed_ += kMinGearRollSpeed_;
			}
			else {
				gearRotateSpeed_ -= kMinGearRollSpeed_;
			}
		}
		// 通常の挙動
		else {
			if (isRotateRight_) {
				gearRotateSpeed_ += kMinGearRollSpeed_;
			}
			else {
				gearRotateSpeed_ -= kMinGearRollSpeed_;
			}
		}
	}
	else {
		if (0 < gearRotateSpeed_) {
			gearRotateSpeed_ -= kGearFriction_ * 0.01f;
		}
		else if (gearRotateSpeed_ < 0) {
			gearRotateSpeed_ += kGearFriction_ * 0.01f;
		}

	}
	// 最大速度
	gearRotateSpeed_ = Math::Clamp(gearRotateSpeed_, -kMaxGearRotateSpeed_, kMaxGearRotateSpeed_);
	// 回転速度を回転に加える
	// ここで速度に応じて回転角に変える
	gearTheta_ += ConvertSpeedToRadian(gearRotateSpeed_);
}

void Player::UpdatePlayerRotate()
{
	// 着地していなかったら処理しない
	if (!isLanding_)
		return;
	// 歯車の回転によって回転の補正をする
	playerTheta_ += ConvertSpeedToRadian(gearRotateSpeed_);
	// float にキャストしたπを取得
	float pi = static_cast<float>(std::numbers::pi);
	// 角度を一回転の中に収める
	if (pi < playerTheta_) {
		playerTheta_ = -pi + (pi - playerTheta_);
	}
	else if (playerTheta_ < -pi) {
		playerTheta_ = pi + (pi + playerTheta_);
	}
	// 位置を変える
	Vector2 direct = { std::cosf(playerTheta_),std::sinf(playerTheta_) };
	direct.x = direct.x * (kGearInnerRadius_ - 0.5f);
	direct.y = direct.y * (kGearInnerRadius_ - 0.5f);
	transform_.translate_ = Vector3(direct.x, direct.y, transform_.translate_.z);
	// ついでに回転させる
	transform_.rotate_.z = playerTheta_ + pi / 2.0f;
}


float Player::ConvertSpeedToRadian(float rotateSpeed) {
	float result = 0;

	float pi = static_cast<float>(std::numbers::pi);

	float circle = 2 * pi * kGearInnerRadius_;

	float arc = rotateSpeed;

	float theta = arc / circle;
	theta = theta * 2 * pi;
	// 回転速度(力)を角度に変える
	//result = rotateSpeed / kGearInnerRadius_;
	result = theta;

	return result;
}
void Player::CheckDirectionRotate()
{
	if (isLanding_) {

		// float にキャストしたπを取得
		float pi = static_cast<float>(std::numbers::pi);

		// ラジアンは右から下への計算だと思うので、計算しやすくする
		// ラジアンの計算は右から上に増える
		float directionRotate = playerTheta_ - pi / 2.0f;
		// プレイヤーの回転角を制限する
		//playerTheta_ = std::fmodf(playerTheta_, pi);
		if (pi < directionRotate) {
			directionRotate = -pi + (pi - directionRotate);
		}
		else if (directionRotate < -pi) {
			directionRotate = pi + (pi + directionRotate);
		}

		//float downStartTheta = 0;


		// 左側にくっついている時
		if (directionRotate < 0) {
			isRotateRight_ = false;
		}
		// 右側にくっついている時
		else {
			isRotateRight_ = true;
		}
	}
}

void Player::CheckGearCollision()
{
	// float にキャストしたπを取得
	//float pi = static_cast<float>(std::numbers::pi);
	// プレイヤーの位置を歯車の中心からの半径として取得する
	//float degree = std::atan2(transform_.translate_.y, transform_.translate_.x);
	// ラジアンを取得
	//float radian = degree * pi / 180;
	float radian = std::atan2(transform_.translate_.y, transform_.translate_.x);
	// プレイヤーの位置がそのままベクトルになる
	Vector3 position = transform_.translate_;
	position.z = 0.0f;
	// 中心からの長さ
	float length = Math::Length(position);
	// 歯車の内円にぶつかっている時
	if (kGearInnerRadius_ <= length) {
		isLanding_ = true;
		playerVelocity_ = { 0.0f,0.0f,0.0f };
		playerTheta_ = radian;
	}
}

void Player::AirJump() {
	playerVelocity_.y += 2.0f;
}

void Player::DebugGui() {
#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());

	ImGui::Separator();

	ImGui::Checkbox("isLanding", &isLanding_);
	ImGui::Checkbox("isJumping", &isJumpTrigger_);
	ImGui::Checkbox("isPendulum", &isPendulum_);
	ImGui::Checkbox("wasRight", &wasRotateRight_);
	//float degree = playerTheta_ * 180 / static_cast<float>(std::numbers::pi);
	//ImGui::SliderAngle("playerTheta", &playerTheta_);
	ImGui::SliderFloat("playerTheta", &playerTheta_, -3.14f, 3.14f);
	ImGui::SliderFloat("rotateSpeed", &gearRotateSpeed_, -kMaxGearRotateSpeed_, kMaxGearRotateSpeed_);
	//playerTheta_ = degree * static_cast<float>(std::numbers::pi) / 180;
	ImGui::Checkbox("isUpdateRotate", &isUpdateRotate);
	if (isUpdateRotate) {
		Vector2 direct = { std::cosf(playerTheta_),std::sinf(playerTheta_) };
		direct.x = direct.x * kGearInnerRadius_;
		direct.y = direct.y * kGearInnerRadius_;
		transform_.translate_ = Vector3(direct.x, direct.y, transform_.translate_.z);
	}
	ImGui::DragFloat("kMinRotate", &kMinGearRollSpeed_, 0.00001f, -0.01f, 0.01f, "%.5f");
	ImGui::DragFloat("kFriction", &kGearFriction_, 0.00001f, -0.01f, 0.01f, "%.5f");
	ImGui::DragFloat("kGravity", &kGravity_, 0.00001f, -1.0f, 1.0f, "%.5f");


	ImGui::End();

#endif // _DEBUG
}
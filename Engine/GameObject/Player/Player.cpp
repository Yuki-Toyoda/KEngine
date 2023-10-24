#include "Player.h"
#include "../Item/Item.h"
#include "../Catapult/Catapult.h"
#include "../../Scene/SceneManager.h"

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

	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();
	// 音量取得
	seVolume_ = &SceneManager::GetInstance()->seVolume_;

	// 効果音読み込み
	soundHandleRotateGear_[0] = audio_->LoadWave("/Audio/SE/RotateGear_0.8.wav"); // ギアの回転音
	soundHandleRotateGear_[1] = audio_->LoadWave("/Audio/SE/RotateGear_0.9.wav"); // ギアの回転音
	soundHandleRotateGear_[2] = audio_->LoadWave("/Audio/SE/RotateGear_1.0.wav"); // ギアの回転音
	soundHandleRotateGear_[3] = audio_->LoadWave("/Audio/SE/RotateGear_1.1.wav"); // ギアの回転音
	soundHandleRotateGear_[4] = audio_->LoadWave("/Audio/SE/RotateGear_1.2.wav"); // ギアの回転音
	playIndex_ = 0;
	isReturn_ = false;
	playSoundAmountRotation_ = 0.0f;
	kPlaySoundRotation_ = 0.3f;
	soundHandleJump_ = audio_->LoadWave("/Audio/SE/Jump.wav"); // ジャンプ音
	soundHandleItemJump_ = audio_->LoadWave("/Audio/SE/ItemJump.wav"); // アイテムでのジャンプ音
	soundHandleCatchCatapult_ = audio_->LoadWave("/Audio/SE/CatchCatapult.wav"); // カタパルトにはまった時の音
	playCatchSound_ = false;
	soundHandleJumpCatapult_ = audio_->LoadWave("/Audio/SE/JumpCatapult.wav"); // カタパルトのジャンプ音

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
	if(isGetOperation_)
		GetOperation();


	// プレイヤーの更新
	UpdatePlayer();
	// 歯車の更新
	UpdateGear();

	// 回転中の位置の補正
	UpdatePlayerRotate();

	DebugGui();

	if (playSoundAmountRotation_ >= kPlaySoundRotation_) {
		audio_->PlayWave(soundHandleRotateGear_[playIndex_], false, *seVolume_ * 0.1f);
		if (isReturn_)
			playIndex_--;
		else
			playIndex_++;

		if (playIndex_ == 4)
			isReturn_ = true;
		else
			isReturn_ = false;

		playSoundAmountRotation_ = 0.0f;
	}
		
#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());
	ImGui::SliderFloat("tempo", &kPlaySoundRotation_, 0.05f, 1.0f);
	ImGui::End();

#endif // _DEBUG


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
	//globalVariables_->AddItem("Gear", "kGearFriction", kGearFriction_);
	globalVariables_->AddItem("Gear", "kGearInnerRadius", kGearInnerRadius_);
	globalVariables_->AddItem("Gear", "kGearAmplitude", kGearAmplitude);
	globalVariables_->AddItem("Gear", "kGearDecreaseRate", kGearDecreaseRate_);
	globalVariables_->AddItem("Gear", "kMinGearPendulumSpeed", kMinGearPendulumSpeed_);
	globalVariables_->AddItem("Gear", "GearScale", Vector2(gearTransform_.scale_.x, gearTransform_.scale_.z));
	globalVariables_->AddItem("Player", "kMaxPlayerVelocity", kMaxPlayerVelocity_);
	globalVariables_->AddItem("Player", "kJumpPoint", Vector2(kJumpPoint_.x, kJumpPoint_.y));
	globalVariables_->AddItem("Player", "kGravity", kGravity_);
	globalVariables_->AddItem("Player", "kPlayerJumpPower", kPlayerJumpPower_);

	globalVariables_->AddItem("Player", "kAirJumpPower", kAirJumpPower_);
	globalVariables_->AddItem("Player", "kCatapultPower", kCatapultPower_);
}

void Player::ApplyGlobalVariables()
{
	kMaxGearRotateSpeed_ = globalVariables_->GetFloatValue("Gear", "kMaxGearRotateSpeed");
	kGearInnerRadius_ = globalVariables_->GetFloatValue("Gear", "kGearInnerRadius");
	kGearAmplitude = globalVariables_->GetFloatValue("Gear", "kGearAmplitude");
	kGearDecreaseRate_ = globalVariables_->GetFloatValue("Gear", "kGearDecreaseRate");
	kMinGearPendulumSpeed_ = globalVariables_->GetFloatValue("Gear", "kMinGearPendulumSpeed");
	kMinGearRollSpeed_ = globalVariables_->GetFloatValue("Gear", "kMinGearRollSpeed");
	Vector2 temp = globalVariables_->GetVector2Value("Gear", "GearScale");
	gearTransform_.scale_ = Vector3(temp.x, temp.x, temp.y);
	kMaxPlayerVelocity_ = globalVariables_->GetFloatValue("Player", "kMaxPlayerVelocity");
	kPlayerJumpPower_ = globalVariables_->GetFloatValue("Player", "kPlayerJumpPower");
	kGravity_ = globalVariables_->GetFloatValue("Player", "kGravity");
	temp = globalVariables_->GetVector2Value("Player", "kJumpPoint");
	kJumpPoint_ = Vector3(temp.x, temp.y, 0.0f);

	kAirJumpPower_ = globalVariables_->GetFloatValue("Player", "kAirJumpPower");
	kCatapultPower_ = globalVariables_->GetFloatValue("Player", "kCatapultPower");

}

void Player::OnCollisionEnter(BaseObject* object)
{
	object;
}

void Player::OnCollision(BaseObject* object)
{
	if (!object->GetIsActive()) {
		return;
	}
	Catapult* catapult = dynamic_cast<Catapult*>(object);

	if (catapult && !playCatchSound_) {
		audio_->PlayWave(soundHandleCatchCatapult_, false, *seVolume_);
		playCatchSound_ = true;
	}

	if (catapult) {
		if (catapult->GetJumpEnable() && input_->TriggerKey(DIK_Q)) {
			// 効果音再生
			audio_->PlayWave(soundHandleJumpCatapult_, false, *seVolume_);
			transform_.translate_ = catapult->transform_.translate_;
			playerTheta_ = catapult->GetTheta();
			CatapultJump();
			catapult->AirJump();
		}
	}
	// ボタンの再入力があった時
	else {
		//object;
		Item* item = dynamic_cast<Item*>(object);
		// 衝突しているのが Item だった時
		// ジャンプできる状態の時
		if (item) {
			if (item->GetJumpEnable()) {
				// カタパルト中は破壊する
				if (isCatapult_) {
					item->AirJump();
				}
				else {
					// ボタンの再入力があった時
					if (input_->TriggerKey(DIK_Q)) {
						// 効果音再生
						audio_->PlayWave(soundHandleItemJump_, false, *seVolume_);
						AirJump();
						item->AirJump();
					}
				}
			}
		}
	}
}

void Player::OnCollisionExit(BaseObject* object)
{
	Catapult* catapult = dynamic_cast<Catapult*>(object);

	if (catapult)
		playCatchSound_ = false;
}

/// プライべート関数

void Player::InitializeVariables()
{

	// 操作を受け付けるか
	isGetOperation_ = false;
	// プレイヤー変数初期化
	playerVelocity_ = { 0.0f,0.0f,0.0f };
	playerAcceleration_ = { 0.0f,0.0f,0.0f };
	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	playerTheta_ = -2;
	radius_ = 1.0f;
	transform_.scale_ = { 1.0f,1.0f,1.0f };

	// 歯車変数初期化
	gearTransform_.Initialize();
	gearTransform_.scale_ = { 8.0f,8.0f,1.0f };
	gearTheta_ = gearTransform_.rotate_.z;
	// 白色不透明
	gearColor_ = color_;
	gearRotateSpeed_ = 0.0f;
	gearRollRatio_ = 1.0f;
	//preGearRotateSpeed_ = 0.0f;

	// フラグ初期化
	isJumpTrigger_ = false;
	isLandLeft_ = true;
	isLanding_ = true;
	isPendulum_ = false;
	wasRotateRight_ = true;
	isCatapult_ = false;

	// 定数の再定義
	kGearInnerRadius_ = 0.5f;
	// さすがに半回転以上は行かない
	kMaxGearRotateSpeed_ = 0.1f;
	kMinGearRollSpeed_ = 0.001f;
	kMinGearPendulumSpeed_ = 1.0f;
	//kGearFriction_ = 0.001f;
	kGearAmplitude = 1.4f;
	kGearDecreaseRate_ = 0.90f;
	kFallDirection_ = { 0.0f,-1.0f,0.0f };
	kJumpPoint_ = { 0.0f,0.0f,0.0f };
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
			isCatapult_ = false;
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
			//playerVelocity_ = { -std::cosf(playerTheta_),-std::sinf(playerTheta_),0.0f };
			Vector3 direct = transform_.translate_ * -1;
			direct = direct + kJumpPoint_;
			direct.z = 0.0f;
			direct = Math::Normalize(direct) * kPlayerJumpPower_;
			playerVelocity_ = direct;
			playerAcceleration_ = { 0.0f,0.0f,0.0f };
			isJumpTrigger_ = false;
			isLanding_ = false;
			isPendulum_ = false;

			// 効果音再生
			audio_->PlayWave(soundHandleJump_, false, *seVolume_);
		}
		// 歯車の回転は歯車の更新内
	}
	else {
		if (!isCatapult_) {
			// 重力減算
			Vector3 fallVelocity{};
			fallVelocity.x = kFallDirection_.x * kGravity_;
			fallVelocity.y = kFallDirection_.y * kGravity_;
			fallVelocity.z = 0.0f;
			playerAcceleration_.x = fallVelocity.x;
			playerAcceleration_.y = fallVelocity.y;
			playerVelocity_.x += playerAcceleration_.x;
			playerVelocity_.y += playerAcceleration_.y;
		}
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
				playerTheta_ < (0 - kGearAmplitude) / 2.0f) {
				// 範囲内に入った瞬間の時振り子フラグ true
				//if (0) {
				//	isPendulum_ = true;
				//	wasRotateRight_ = isRotateRight_;
				//	// 速度が足りない時は速度をプラスする
				//	if (-kMinGearPendulumSpeed_ < gearRotateSpeed_ && gearRotateSpeed_ < kMinGearPendulumSpeed_) {
				//		if (isRotateRight_) {
				//			gearRotateSpeed_ = kMinGearPendulumSpeed_;
				//		}
				//		else {
				//			gearRotateSpeed_ = -kMinGearPendulumSpeed_;
				//		}
				//	}
				//}
				// 速度が足りない時は速度をプラスする
				if (-kMinGearPendulumSpeed_ < gearRotateSpeed_ && gearRotateSpeed_ < kMinGearPendulumSpeed_) {
					if (isLandLeft_) {
						gearRotateSpeed_ += kMinGearRollSpeed_ * gearRollRatio_;
					}
					else {
						gearRotateSpeed_ -= kMinGearRollSpeed_ * gearRollRatio_;
					}
				}
			}
			// 範囲外の時
			if (playerTheta_ < (-pi - kGearAmplitude) / 2.0f ||
				(0 - kGearAmplitude) / 2.0f < playerTheta_) {
				isPendulum_ = false;
			}
		}

		// プレイヤーがくっついている時
		//if (isLanding_) {
			// 速度を加算する
		// 通常の挙動
		if (isLandLeft_) {
			gearRotateSpeed_ += kMinGearRollSpeed_;
		}
		else {
			gearRotateSpeed_ -= kMinGearRollSpeed_;
		}
	}
	else {
		if (isLandLeft_) {
			gearRotateSpeed_ -= kMinGearRollSpeed_ * 0.01f;
		}
		else {
			gearRotateSpeed_ += kMinGearRollSpeed_ * 0.01f;
		}

	}
	// 最大速度
	gearRotateSpeed_ = Math::Clamp(gearRotateSpeed_, -kMaxGearRotateSpeed_, kMaxGearRotateSpeed_);
	// 回転速度を回転に加える
	// ここで速度に応じて回転角に変える
	gearTheta_ += ConvertSpeedToRadian(gearRotateSpeed_);
	if (ConvertSpeedToRadian(gearRotateSpeed_) < 0)
		playSoundAmountRotation_ += ConvertSpeedToRadian(gearRotateSpeed_) * -1.0f;
	else
		playSoundAmountRotation_ += ConvertSpeedToRadian(gearRotateSpeed_);
	
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


		// 右側にくっついている時
		if (directionRotate < 0) {
			isLandLeft_ = false;
		}
		// 左側にくっついている時
		else {
			isLandLeft_ = true;
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
		CheckDirectionRotate();
	}
}

void Player::AirJump() {
	Vector3 direct{};
	float pi = static_cast<float>(std::numbers::pi);
	float pi4 = pi / 3.0f;
	// 左にくっついている時
	if (isLandLeft_) {
		direct = { std::cosf(pi4),std::sinf(pi4),0.0f };
	}
	else {
		direct = { std::cosf(pi - pi4),std::sinf(pi - pi4),0.0f };
	}
	playerVelocity_ = direct * kAirJumpPower_;
}

void Player::CatapultJump()
{
	Vector3 direct = { -std::cosf(playerTheta_),-std::sinf(playerTheta_), 0.0f };
	playerVelocity_ = direct * kCatapultPower_;
	isCatapult_ = true;
}

void Player::DebugGui() {
#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());

	ImGui::Separator();

	ImGui::Checkbox("isLanding", &isLanding_);
	ImGui::Checkbox("isJumping", &isJumpTrigger_);
	//ImGui::Checkbox("isPendulum", &isPendulum_);
	ImGui::Checkbox("isLandLeft", &isLandLeft_);
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
	//ImGui::DragFloat("kFriction", &kGearFriction_, 0.00001f, -0.01f, 0.01f, "%.5f");
	ImGui::DragFloat("kGravity", &kGravity_, 0.00001f, -1.0f, 1.0f, "%.5f");


	ImGui::End();

#endif // _DEBUG
}
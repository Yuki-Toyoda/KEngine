#include "Roller.h"

void Roller::Init()
{
	// 速度ベクトルを既定値でリセット
	velocity_ = { 1.0f,0.0f,0.0f };

	// 各種ワールドトランスフォームの初期化
	modelTransform_.Init();
	modelTransform_.SetParent(&transform_, 0b011);

	// メッシュの追加
	AddMesh(&modelTransform_, color_, "./Resources/Cucumber", "Cucumber.obj");
	// AABBのコライダーを追加
	AddColliderAABB("Roller", &transform_.translate_, &transform_.scale_);
	
	// 大きさ設定
	transform_.scale_ = { 8.0f,1.0f,1.0f };
	// 座標の高さを設定
	transform_.translate_.y = 2.0f;

	isFall_ = true;

	fallTimer_.Start(1.5f);
	
}

void Roller::Update()
{
	// 移動加速度をゲームマネージャーから取得
	moveAcceleration_ = gameManager_->RollerSpeed_;

	fallTimer_.Update();

	// z軸方向のスケールが一定以上の場合
	if (transform_.scale_.z >= 2.0f) {
		// 回転させる
		transform_.rotate_.y = (float)std::numbers::pi / 2.0f;
	}

	// 動作トリガーがtrueの時
	if (isMove_ && !isFall_) {
		// ?????????????
		if (std::fabsf(velocity_.x) > std::fabsf(velocity_.z)) {

			transform_.rotate_.x = 0.0f;
			if (velocity_.x >= 0.0f) {
				transform_.rotate_.z -= 0.1f;
			}
			else {
				transform_.rotate_.z += 0.1f;
			}
		}
		else {
			transform_.rotate_.z = 0.0f;
			if (velocity_.z >= 0.0f) {
				transform_.rotate_.x += 0.1f;
			}
			else {
				transform_.rotate_.x -= 0.1f;
			}
		}
		transform_.translate_ += Math::Normalize(velocity_) * moveAcceleration_;
	}
	if (isFall_) 
	{
		transform_.translate_.y = KLib::Lerp<float>(50.0f, 2.0f, KLib::EaseInQuad(fallTimer_.GetProgress()));
	}

	if (fallTimer_.GetIsFinish()) {
		isFall_ = false;
	}

	if (std::abs(transform_.translate_.x) >= 60.0f || std::abs(transform_.translate_.z) >= 60.0f) {
		Destroy();
		return;
	}
}

void Roller::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
}

void Roller::OnCollisionEnter(Collider* collider)
{
	collider;
}

void Roller::SetVelocity(const Vector3& velocity)
{
	// 速度ベクトルをセット
	velocity_ = velocity;
	// ローラーの向きを速度ベクトルに合わせて回転させる
	//transform_.rotate_.y = std::atan2(velocity_.x, velocity_.z);
}

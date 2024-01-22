#include "Player.h"

void Player::Init()
{
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };
	// 球のコライダーを追加
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
	ChangeState(std::make_unique<RootState>());
}

void Player::Update()
{

	// 行動状態クラスがあれば
	if (state_.get()) {
		// 行動状態の更新を行う
		state_->Update();
	}
	hitCollTimer_.Update();
	prevPos_ = worldPos_;
	worldPos_ = transform_.translate_;
	
	transform_.translate_ = transform_.translate_ +velocity_;
	//地面より外に出たら中に戻して状態をRootに変更
	if (transform_.translate_.x+transform_.scale_.x >= ground_->transform_.scale_.x|| transform_.translate_.x - transform_.scale_.x <= -ground_->transform_.scale_.x) {
		velocity_.x =0.0f;
		transform_.translate_.x = prevPos_.x;
		ChangeState(std::make_unique<RootState>());
		return;
	}
	if (transform_.translate_.z + transform_.scale_.z >= ground_->transform_.scale_.z || transform_.translate_.z - transform_.scale_.z <= -ground_->transform_.scale_.z) {
		velocity_.z =0.0f;
		transform_.translate_.z = prevPos_.z;
		ChangeState(std::make_unique<RootState>());
		return;
	}
	//SubtractVelocity();
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
	//減速率
	ImGui::DragFloat("deceleration rate", &decelerationRate, 0.01f);
	//拡大率
	ImGui::DragFloat("scale rate", &scaleForce_, 0.01f);
	//攻撃倍率
	ImGui::DragFloat("atack rate", &atackForce_, 0.01f);
}

void Player::OnCollisionEnter(Collider* collider)
{
	if (state_->name_ == "Root") {
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagRubble) {
			//がれきにぶつかったらしてサイズを大きくする
			absorptionCount_++;
			float addSize = absorptionCount_ * scaleForce_;
			transform_.translate_.y += addSize;
			transform_.scale_ = { transform_.scale_.x + addSize,transform_.scale_.y + addSize,transform_.scale_.z + addSize };

		}
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagMeteor) {
			Damage();
		}
		
	}
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy && isAtack_) {
		//吸収した数をリセットして座標とスケール調整
		ResetAbsorptionCount();

		transform_.translate_.y = 2.0f;
		transform_.scale_ = { 1.0f,1.0f ,1.0f };
		isAtack_ = false;
		velocity_ = { 0.0f,0.0f,0.0f };
		transform_.translate_ = prevPos_;
		ChangeState(std::make_unique<BlowAwayState>());
		return;
	}
}

void Player::Damage()
{
	//クールタイムを過ぎていたら攻撃をくらう
	if (hitCollTimer_.GetIsFinish()) {
		hitPoint_--;
		hitCollTimer_.Start(hitCoolTime_);
	}

}
void Player::SubtractVelocity()
{
	velocity_ = velocity_ * decelerationRate;
	if (std::abs(velocity_.x) <= 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::abs(velocity_.z) <= 0.01f) {
		velocity_.z = 0.0f;
	}
}

void Player::Atack()
{
	
	//攻撃力を吸収した数*攻撃倍率に
	atackPower_ =static_cast<float>( absorptionCount_) * atackForce_;
	velocity_ = Math::Normalize(  Vector3(0.0f, transform_.translate_.y, 0.0f)- transform_.translate_)*2.0f;


}

void Player::ChangeState(std::unique_ptr<IPlayerState> newState)
{
	// 共通初期化関数を実行
	newState->PreInit(this);
	// 初期化関数を実行
	newState->Init();

	// 新しい行動状態を受け渡す
	state_ = std::move(newState);
}

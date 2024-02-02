#include "DashState.h"
#include "../Player.h"

void DashState::Init()
{
	name_ = "Dash";

	// 元の速さ
	velocity_ = player_->GetMoveDirect();
	// 進ませる方向ベクトル
	moveDirect_ = Math::Normalize(player_->GetMoveDirect());
	// 終了フレーム
	axelTimer_ = 0.3f;
	brakeTimer_ = 0.15f;
	dashPower_ = 7.5f;
	// 加速と減速の段階を管理する
	moveStep_ = kAccleratorStep;
	// 開始
	dashTimer_.Start(axelTimer_);
}

void DashState::Update()
{
	// 更新
	dashTimer_.Update();


	// 加速の終了するタイミング
	if (dashTimer_.GetIsFinish() && moveStep_ == 0) {

		dashTimer_.Start(brakeTimer_);
		// 最大速度設定
		maxVelocity_ = velocity_;
		// 状態を変更
		moveStep_ = kDecelerationStep;
	}
	// 加速処理
	if(dashTimer_.GetIsActive() && moveStep_ == kAccleratorStep) {
		velocity_ += (moveDirect_ * player_->GetMoveAcceleration()) * dashPower_;
	}
	// 減速の終了タイミング（ステートが戻る
	else if (dashTimer_.GetIsActive() && moveStep_ == kDecelerationStep) {
		velocity_ = KLib::Lerp<Vector3>(maxVelocity_, { 0,0,0 }, dashTimer_.GetProgress());
	}
	// 減速処理
	if (dashTimer_.GetIsFinish() && moveStep_ == kDecelerationStep) {
		player_->ChangeState(std::make_unique<RootState>());
		return;
	}

	player_->SetVelocity(velocity_);

}

void DashState::DisplayImGui()
{
}

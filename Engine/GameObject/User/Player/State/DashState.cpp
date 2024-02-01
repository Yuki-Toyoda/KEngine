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
	float endFrame = 5.0f;
	// 加速と減速の段階を管理する
	endCount_ = 0;
	// 開始
	dashTimer_.Start(1.0f / endFrame);
}

void DashState::Update()
{
	// 更新
	dashTimer_.Update();
	// ダッシュのパワー
	float dashPower = 1.5f;

	// 加速の終了するタイミング
	if (dashTimer_.GetIsFinish() && endCount_ == 0) {

		dashTimer_.Start(1.0f / 5.0f);

		endCount_++;
		maxVelocity_ = velocity_;
	}
	// 加速処理
	if(dashTimer_.GetIsActive() && endCount_ == 0) {
		velocity_ += (moveDirect_ * player_->GetMoveAcceleration()) * dashPower;
	}
	// 減速の終了タイミング（ステートが戻る
	else if (dashTimer_.GetIsActive() && endCount_ == 1) {
		velocity_ = KLib::Lerp<Vector3>(maxVelocity_, { 0,0,0 }, dashTimer_.GetProgress());
	}
	// 減速処理
	if (dashTimer_.GetIsFinish() && endCount_ == 1) {
		player_->ChangeState(std::make_unique<RootState>());
		return;
	}

	player_->SetVelocity(velocity_);

}

void DashState::DisplayImGui()
{
}

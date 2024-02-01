#include "WallCollisionState.h"
#include "../Player.h"

void WallCollisionState::Init()
{
	// ステート名の初期化
	name_ = "WallCollision";

	// めり込み座標
	startPosition = player_->transform_.translate_;
	// 修正座標
	correctPosition = player_->GetCorrectPosition();

	// 終了フレーム数
	GlobalVariables* variables = GlobalVariables::GetInstance();
	float endFrame = variables->GetFloatValue(player_->GetObjectName(), "correctFrame");
	// 終了時間の設定
	lerpTimer_ = KLib::DeltaTimer(1.0f / endFrame);
	lerpTimer_.Start();
}

void WallCollisionState::Update()
{
	// 移動しないように初期化
	velocity_ = {};
	player_->SetVelocity({});
	lerpTimer_.Update();
	// 補間の処理
	if (!lerpTimer_.GetIsFinish())
	{
		// 座標の修正処理
		player_->transform_.translate_ = KLib::Lerp(startPosition, correctPosition, lerpTimer_.GetProgress());
	}
	else
	{
		// ステートを戻す
		player_->ChangeState(std::make_unique<RootState>());
	}

}

void WallCollisionState::DisplayImGui()
{


}

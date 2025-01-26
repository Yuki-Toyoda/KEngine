#include "EnemyHideAttack.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/Utility/Random/Random.h"

void EnemyHideAttack::Init()
{
	// 行動名設定
	stateName_ = "HideAttack";

	// 敵をロックオン不可能な状態に
	enemy_->SetCanLockOn(false);
	// 演出タイマー開始
	stagingTimer_.Start(hideStartTime);

	// 敵から放出するパーティクルのエミッタを調整
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 35;
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequency = 0.05f;
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequencyTime = 0.05f;

	// コライダーの有効状態の切り替え
	enemy_->SetIsActiveCQCCollider(true);

	// 回転固定解除
	enemy_->SetIsRotateLock(false);
}

void EnemyHideAttack::Update()
{
	// 現在の進捗によって処理を変更
	switch (process_)
	{
	case EnemyHideAttack::HIDESTART: // ハイド開始
		HideStartUpdate();
		break;
	case EnemyHideAttack::HIDING: // ハイド中
		HidingUpdate();
		break;
	case EnemyHideAttack::APPEAR: // 出現
		AppearUpdate();
		break;
	case EnemyHideAttack::WAIT: // 攻撃前待機
		WaitUpdate();
		break;
	case EnemyHideAttack::ATTACK: // 攻撃
		AttackUpdate();
		break;
	}

	// 演出タイマー更新
	stagingTimer_.Update();
}

void EnemyHideAttack::DisplayImGui()
{

}

void EnemyHideAttack::HideStartUpdate()
{
	// 全モデルをディゾルブで消していく
	for (std::map<std::string, SkiningModel*>::const_iterator it = enemy_->skiningModels_.cbegin(); it != enemy_->skiningModels_.cend(); ++it) {
		// マテリアルに色を適用
		it->second->materials_[0].dissolveStrength_ = stagingTimer_.GetProgress();
		it->second->materials_[0].dissolveEdgeColor_ = dissolveEdgeColor_;
		it->second->materials_[0].dissolveEdgeThreshold_ = dissolveEdgeThreshold_;
	}

	// 演出タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 敵から放出するパーティクルを停止
		enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 0;

		// 演出タイマーを再度開始
		stagingTimer_.Start(hidingTime);

		// 隠れきったらアニメーションを変更
		enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("Rally");
		// 再生は停止させておく
		enemy_->skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(0.0f);

		// 進捗を進める
		process_++;
	}
}

void EnemyHideAttack::HidingUpdate()
{
	// プレイヤーの座標と回転を取得
	Vector3 playerPosition = enemy_->GetPlayer()->transform_.translate_;
	Vector3 playerRotate = enemy_->GetPlayer()->transform_.rotate_;

	// プレイヤーのY軸の回転を元に回転行列を生成
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(playerRotate.y);
	// プレイヤーとのオフセットの計算
	Vector3 offset = kDistanceOffset * rotateMat;
	// 敵の座標をプレイヤーの後ろに設定
	enemy_->transform_.translate_ = playerPosition + offset;

	// 移動方向の指定
	velocity_ = Vector3(0.0f, 0.0f, 1.0f) * rotateMat;

	// 演出タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 敵から放出するパーティクルのエミッタを調整
		enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 35;

		// 演出タイマーを再度開始
		stagingTimer_.Start(appearTime);

		// ロックオンを可能な状態に
		enemy_->SetCanLockOn(true);

		// カウンター可能状態に
		enemy_->SetIsCanCounter(true);

		// 敵の回転をロック
		enemy_->SetIsRotateLock(true);

		// 進捗を進める
		process_++;
	}
}

void EnemyHideAttack::AppearUpdate()
{
	// 全モデルをディゾルブで出現させる
	for (std::map<std::string, SkiningModel*>::const_iterator it = enemy_->skiningModels_.cbegin(); it != enemy_->skiningModels_.cend(); ++it) {
		// マテリアルに色を適用
		it->second->materials_[0].dissolveStrength_ = 1.0f - stagingTimer_.GetProgress();
		it->second->materials_[0].dissolveEdgeColor_ = dissolveEdgeColor_;
		it->second->materials_[0].dissolveEdgeThreshold_ = dissolveEdgeThreshold_;
	}

	// 演出タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 敵から放出するパーティクルのエミッタを調整
		enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 25;

		// 演出タイマーを再度開始
		stagingTimer_.Start(waitTime);

		// 進捗を進める
		process_++;
	}
}

void EnemyHideAttack::WaitUpdate()
{
	// 演出タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 攻撃アニメーション再生開始
		enemy_->skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(1.0f);

		// カウンターを不可能に
		enemy_->SetIsCanCounter(false);

		// 演出タイマーを再度開始
		stagingTimer_.Start(attackTime);

		// 進捗を進める
		process_++;
	}
}

void EnemyHideAttack::AttackUpdate()
{
	// 現在のアニメーション進捗を取得
	float animationProgress = enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationProgress();

	// アニメーション進捗を元に攻撃判定を付ける
	if (animationProgress >= 0.25f && animationProgress <= 0.75f) {
		enemy_->SetIsCQCAttack(true);

		// 攻撃中は移動させる
		enemy_->transform_.translate_ += velocity_ * speed;
	}
	else {
		enemy_->SetIsCQCAttack(false);
	}

	// 演出タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 待機状態へ
		enemy_->ChangeState(std::make_unique<EnemyRoot>());
		// それ以降の処理を無視
		return;
	}
}

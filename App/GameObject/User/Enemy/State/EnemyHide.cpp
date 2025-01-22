#include "EnemyHide.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/Utility/Random/Random.h"

void EnemyHide::Init()
{
	// 行動名設定
	stateName_ = "Hide";

	// ロックオンを不可能な状態に
	enemy_->SetCanLockOn(false);
	// 演出タイマーを開始
	stagingTimer_.Start(hideStartTime);

	// 敵から放出するパーティクルのエミッタを調整
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 35;
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequency = 0.05f;
	enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequencyTime = 0.05f;
}

void EnemyHide::Update()
{
	switch (progress_)
	{
	case EnemyHide::HIDESTART: // 隠れ開始
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
			stagingTimer_.Start(hideTime);

			// 進捗を進める
			progress_++;
		}
		break;
	case EnemyHide::HIDING: // 隠れ中
		// プレイヤーの座標取得
		playerPosition_ = enemy_->GetPlayer()->transform_.translate_;

		// プレイヤーから一定の距離を取るように調整
		sub_ = Vector3::Normalize(enemy_->transform_.translate_ - playerPosition_);

		// 移動座標を求める
		enemy_->transform_.translate_ = sub_ * 20.0f;

		// 演出タイマー終了時
		if (stagingTimer_.GetIsFinish()) {
			// 敵から放出するパーティクルのエミッタを調整
			enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 35;

			// 演出タイマーを再度開始
			stagingTimer_.Start(hideEndTime);

			// 進捗を進める
			progress_++;
		}
		break;
	case EnemyHide::HIDEEND: // 隠れ終了
		// 全モデルをディゾルブで消していく
		for (std::map<std::string, SkiningModel*>::const_iterator it = enemy_->skiningModels_.cbegin(); it != enemy_->skiningModels_.cend(); ++it) {
			// マテリアルに色を適用
			it->second->materials_[0].dissolveStrength_ = 1.0f - stagingTimer_.GetProgress();
			it->second->materials_[0].dissolveEdgeColor_ = dissolveEdgeColor_;
			it->second->materials_[0].dissolveEdgeThreshold_ = dissolveEdgeThreshold_;
		}

		// 演出タイマー終了時
		if (stagingTimer_.GetIsFinish()) {
			// 敵から放出するパーティクルを停止
			enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->count = 25;
			enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequency = 0.1f;
			enemy_->GetEnemyParticle()->emitterDataBuffer_->data_->frequencyTime = 0.1f;

			// ロックオンを可能な状態に
			enemy_->SetCanLockOn(true);

			// 行動を変更
			enemy_->ChangeState(std::make_unique<EnemyRoot>());
			// それ以降の処理を無視
			return;
		}

		break;
	}

	// 演出タイマー更新
	stagingTimer_.Update();
}

void EnemyHide::DisplayImGui()
{
}

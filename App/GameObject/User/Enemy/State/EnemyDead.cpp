#include "EnemyDead.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"

void EnemyDead::Init()
{
	// 行動名設定
	stateName_ = "Dead";

	// 麻痺パーティクルのインスタンスが存在する場合
	if (enemy_->GetStunParticle() != nullptr) {
		// 強制敵に停止
		enemy_->GetStunParticle()->SetIsEnd(true);
	}

	// 全パーティクルの再生再開
	ParticleManager::GetInstance()->SetTimeScale(1.0f);

	// 死亡アニメーションを再生
	enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("Dead");
	enemy_->transform_.translate_ = { 0.0f, 0.0f, -0.5f };
	enemy_->transform_.rotate_.y = 0.037f;

	// プレイヤー強制移動
	Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	p->transform_.translate_ = { -0.544f, 0.0f, -4.65f };
	p->transform_.rotate_.y = 0.28f;

	// 敵が死亡した時点で行動不可能な状態にする
	p->SetCanAction(false);
	
	// 待機アニメーション再生
	p->skiningModels_["Player"]->animationManager_.PlayAnimation("Idle", 0.0f, true);

	// カメラ移動
	camera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
	camera_->transform_.translate_ = { -2.777f, 0.3f, -6.4f };
	camera_->transform_.rotate_ = { -0.285f, 0.55f, 0.0f };
	camera_->fov_ = 0.6f;
	camera_->UseThisCamera();
}

void EnemyDead::Update()
{
	// アニメーションが終了していたら
	if (!enemy_->skiningModels_["Enemy"]->animationManager_.GetIsPlayingAnimation()) {
		// ディゾルブ演出が開始していなければ
		if (!isStartDissolveStaging_) {
			// 演出タイマー開始
			dissolveStagingTimer_.Start(dissolveStagingTime_);
			// ディゾルブ演出開始フラグTrue
			isStartDissolveStaging_ = true;
		}
	}

	// ディゾルブ演出が開始されている場合
	if (isStartDissolveStaging_) {
		// 全モデルをディゾルブで消していく
		for (std::map<std::string, SkiningModel*>::const_iterator it = enemy_->skiningModels_.cbegin(); it != enemy_->skiningModels_.cend(); ++it) {
			// マテリアルに色を適用
			it->second->materials_[0].dissolveStrength_ = dissolveStagingTimer_.GetProgress();
			it->second->materials_[0].dissolveEdgeColor_ = deadDissolveEdgeColor_;
			it->second->materials_[0].dissolveEdgeThreshold_ = deadDissolveEdgeThreshold_;
		}

		// ディゾルブ演出タイマー更新
		dissolveStagingTimer_.Update();
	}

	// ディゾルブ演出が終了、かつディゾルブ演出が開始されている場合
	if (dissolveStagingTimer_.GetIsFinish() && isStartDissolveStaging_) {
		// 死亡状態に
		enemy_->SetIsDead(true);
	}
}

void EnemyDead::DisplayImGui()
{
}

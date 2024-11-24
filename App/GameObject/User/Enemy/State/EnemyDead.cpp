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

	// ループを切れる
	anim_->isLoop_ = false;
	// 敵のアニメーションを変更
	anim_->ChangeParameter("Enemy_Dead", true);
	enemy_->transform_.rotate_.y = 0.037f;

	// プレイヤー強制移動
	Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	p->transform_.translate_ = { -0.544f, 0.0f, -4.65f };
	p->transform_.rotate_.y = 0.28f;

	// 敵が死亡した時点で行動不可能な状態にする
	p->SetCanAction(false);
	
	// 待機アニメーション再生
	p->skiningModels_["Player"]->animationManager_.PlayAnimation("00_Idle", 0.0f, true);

	// カメラ移動
	camera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
	camera_->transform_.translate_ = { -2.777f, 0.3f, -6.4f };
	camera_->transform_.rotate_ = { -0.285f, 0.55f, 0.0f };
	camera_->fov_ = 0.6f;
	camera_->UseThisCamera();
	camera_->ppProcessor_.bloom_.threshold_ = 0.25f;
}

void EnemyDead::Update()
{
	// 全通常モデルを透明に
	for (std::map<std::string, NormalModel*>::const_iterator it = enemy_->normalModels_.cbegin(); it != enemy_->normalModels_.cend(); ++it) {
		// マテリアルに色を適用
		it->second->materials_[1].dissolveStrength_ = enemy_->GetColor().w;
		it->second->materials_[1].dissolveEdgeColor_ = { 0.0f, 1.0f, 0.75f };
		it->second->materials_[1].dissolveEdgeThreshold_ = 0.025f;
	}

	// アニメーションが終了していたら
	if (anim_->isEnd_) {
		// 死亡状態に
		enemy_->SetIsDead(true);
	}
}

void EnemyDead::DisplayImGui()
{
}

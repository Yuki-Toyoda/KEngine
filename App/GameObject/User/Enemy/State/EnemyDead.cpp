#include "EnemyDead.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/GameObject/Core/Camera.h"

void EnemyDead::Init()
{
	// 行動名設定
	stateName_ = "Dead";

	// ループを切れる
	enemy_->enemyAnim_->isLoop_ = false;
	// 敵のアニメーションを変更
	enemy_->enemyAnim_->ChangeParameter("Enemy_Dead", true);
	enemy_->transform_.rotate_.y = 0.037f;

	// プレイヤー強制移動
	Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	p->transform_.translate_ = { -0.544f, 0.0f, -4.65f };
	p->transform_.rotate_.y = 0.28f;
	p->canMove_ = false;
	
	// 待機アニメーション再生
	p->skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", 0.0f, true);

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
	// 全メッシュ分ループ
	for (int i = 0; i < enemy_->normalModels_.size(); i++) {
		// マテリアルに色を適用
		enemy_->normalModels_[i]->materials_[1].dissolveStrength_ = enemy_->GetColor().w;
		enemy_->normalModels_[i]->materials_[1].dissolveEdgeColor_ = {0.0f, 1.0f, 0.75f};
		enemy_->normalModels_[i]->materials_[1].dissolveEdgeThreshold_ = 0.025f;
	}

	// カメラのポストプロセスの強さをだんだん上げてく
	//camera_->postProcessIntensity_ = KLib::Lerp<float>(0.0f, 1.0f, KLib::EaseInQuad(enemy_->enemyAnim_->GetAnimationProgress()));

	// アニメーションが終了していたら
	if (enemy_->enemyAnim_->isEnd_) {

		if (enemy_->player_->sprites_[12]->color_.w >= 1.0f) {
			// フェードアウト値固定
			enemy_->player_->sprites_[12]->color_.w = 1.0f;
			
			// 死亡している
			enemy_->isDead_ = true;
		}
		else {
			enemy_->player_->sprites_[12]->color_.w += 0.01f;
		}
	}
}

void EnemyDead::DisplayImGui()
{
}

#include "EnemyDead.h"
#include "../Enemy.h"
#include "../../../GameObjectManager.h"
#include "../../Player/Player.h"
#include "../../../Core/Camera.h"

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
	p->skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", true);

	// カメラ移動
	camera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
	camera_->transform_.translate_ = { -2.777f, 0.3f, -6.4f };
	camera_->transform_.rotate_ = { -0.285f, 0.55f, 0.0f };
	camera_->fov_ = 0.6f;
	camera_->UseThisCamera();
}

void EnemyDead::Update()
{
	// 全メッシュ分ループ
	for (int i = 0; i < enemy_->normalModels_.size(); i++) {
		// マテリアルに色を適用
		enemy_->normalModels_[i]->materials_[1].color_ = enemy_->GetColor();
	}

	// カメラのポストプロセスの強さをだんだん上げてく
	//camera_->postProcessIntensity_ = KLib::Lerp<float>(0.0f, 1.0f, KLib::EaseInQuad(enemy_->enemyAnim_->GetAnimationProgress()));

	// アニメーションが終了していたら
	if (enemy_->enemyAnim_->isEnd_) {

		if (camera_->ppProcessor_.hsvFilter_.hsv_.value <= -1.0f) {
			// フェードアウト値固定
			camera_->ppProcessor_.hsvFilter_.hsv_.value = -1.0f;
			
			// 死亡している
			enemy_->isDead_ = true;
		}
		else {
			camera_->ppProcessor_.hsvFilter_.hsv_.value -= 0.01f;
		}
	}
}

void EnemyDead::DisplayImGui()
{
}

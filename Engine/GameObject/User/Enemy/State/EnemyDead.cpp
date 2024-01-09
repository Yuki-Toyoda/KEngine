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

	// カメラ移動
	Camera* c = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", BaseObject::TagCamera);
	c->transform_.translate_ = { -2.777f, 0.3f, -6.4f };
	c->transform_.rotate_ = { -0.285f, 0.55f, 0.0f };
	c->fov_ = 0.6f;
	c->UseThisCamera();
}

void EnemyDead::Update()
{
	// アニメーションが終了していたら
	if (enemy_->enemyAnim_->isEnd_) {
		// 死亡している
		enemy_->isDead_ = true;
	}
}

void EnemyDead::DisplayImGui()
{
}

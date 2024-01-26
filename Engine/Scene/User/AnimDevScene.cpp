#include "AnimDevScene.h"

void AnimDevScene::Init()
{
	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* pam = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	pam->transform_.translate_.x = 3.0f;
	pam->CrateAnimation();

	// プレイヤーアニメーションマネージャの生成
	BossAnimManager* eam_ = gameObjectManager_->CreateInstance<BossAnimManager>("bossAnim", BaseObject::TagEnemy);
	// アニメーション生成
	eam_->CrateAnimation();

	// 地面生成
	Ground* ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	// 座標調整
	ground->transform_.translate_ = { 0.0f, -1.0f, 0.0f };
	// スケール調整
	ground->transform_.scale_ = { 55.81f,1.0f,32.5f };
}

void AnimDevScene::Update()
{

}

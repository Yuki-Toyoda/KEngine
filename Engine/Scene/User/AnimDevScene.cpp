#include "AnimDevScene.h"

void AnimDevScene::Init()
{
	// インゲームカメラの生成
	Camera* cam = gameObjectManager_->CreateInstance<Camera>("InGameCamera", BaseObject::TagCamera);
	cam->fov_ = 0.85f;
	cam->transform_.translate_ = { 0.0f,47.0f,-90.0f };
	cam->transform_.rotate_ = { 0.55f,0.0f,0.0f };

	// 地面生成
	Ground* ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	// 座標調整
	ground->transform_.translate_ = { 0.0f, -1.0f, 0.0f };
	// スケール調整
	ground->transform_.scale_ = { 55.81f,1.0f,32.5f };

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* pam = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	pam->transform_.translate_.x = 3.0f;
	pam->CreateAnimation();

	// プレイヤーアニメーションマネージャの生成
	BossAnimManager* eam_ = gameObjectManager_->CreateInstance<BossAnimManager>("bossAnim", BaseObject::TagEnemy);
	
	// アニメーション生成
	eam_->CreateAnimation();

}

void AnimDevScene::Update()
{

}

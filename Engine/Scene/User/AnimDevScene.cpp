#include "AnimDevScene.h"

void AnimDevScene::Init()
{
	// プレイヤーアニメーションマネージャの生成
	pam_ = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// アニメーション生成
	pam_->CrateAnimation();

	// 地面生成
	Ground* ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	// 座標調整
	ground->transform_.translate_ = { 0.0f, -2.0f, 0.0f };
	// スケール調整
	ground->transform_.scale_ = { 55.81f,1.0f,32.5f };
}

void AnimDevScene::Update()
{
	if (pam_->GetAnimation()->GetReadingParameterName() == "Player_Stun2") {
		//pam_->transform_.rotate_.y += 0.001f;
	}

}

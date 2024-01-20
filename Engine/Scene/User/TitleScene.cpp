#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleScene::Init()
{
	// カメラの生成
	Camera* c = gameObjectManager_->CreateInstance<Camera>("MainCamera", BaseObject::TagCamera);
	// このカメラを使用
	c->UseThisCamera();
	// 視野角変更
	c->fov_ = 0.6f;
	// カメラ座標を動かす
	c->transform_.translate_ = { 0.0f, 0.0f, -5.0f };
	
	// 天球生成
	gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤーモデルの生成
	PlayerAnimManager* pm = gameObjectManager_->CreateInstance<PlayerAnimManager>("PlayerModel", BaseObject::TagNone);
	// 座標を動かす
	pm->transform_.translate_ = { -1.35f, -0.35f, 0.0f };
	// 回転させる
	pm->transform_.rotate_ = { 0.0f, -0.3f, 0.0f };
}

void TitleScene::Update()
{
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}

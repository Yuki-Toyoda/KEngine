#include "ResultScene.h"
#include "../FadeManager.h"
#include "../SceneManager.h"

// 静的なメンバ変数の実体を宣言
bool ResultScene::isClear_ = false;

void ResultScene::Init()
{
	// カメラ生成
	Camera* c = gameObjectManager_->CreateInstance<Camera>("Incamera", BaseObject::TagCamera);
	// このカメラを使用
	c->UseThisCamera();
	// FOV設定
	c->fov_ = 0.6f;
	// 位置設定
	c->transform_.translate_ = { 0.0f, 1.0f, -5.0f };

	// スカイドーム生成
	SkyDome* skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤーアニメーションマネージャ
	PlayerAnimManager* pam = gameObjectManager_->CreateInstance<PlayerAnimManager>("PlayerAnimManager", BaseObject::TagNone);
	pam->transform_.translate_ = { -1.35f, 1.0f, 0.0f };
	pam->transform_.rotate_ = { 0.0f, -0.35f, 0.0f };

	// リザルトマネージャ生成
	rm_ = gameObjectManager_->CreateInstance<ResultManager>("ResultManager", BaseObject::TagNone);
	// リザルトマネージャに各種オブジェクトをセット
	rm_->SetCamera(c);
	rm_->SetSkyDome(skyDome);
	rm_->SetPlayerAnimManager(pam);
	rm_->PostInit(isClear_);

	// フェードイン
	if (isClear_) {
		// フェードイン(白)
		FadeManager::GetInstance()->ChangeParameter("WhiteIn", true);
	}
	else {
		// フェードイン
		FadeManager::GetInstance()->ChangeParameter("FadeIn", true);
	}
	// フェード演出再生
	FadeManager::GetInstance()->Play();
}

void ResultScene::Update()
{

	// リザルトマネージャの状態で遷移
	if (rm_->GetIsSceneChange()) {
		if (rm_->GetIsRetry()) {
			BaseScene* nextScene = new GameScene();
			SceneManager::GetInstance()->SetNextScene(nextScene);
			
		}
		else {
			BaseScene* nextScene = new TitleScene();
			SceneManager::GetInstance()->SetNextScene(nextScene);
		}
	}

}
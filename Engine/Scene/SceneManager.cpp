#include "SceneManager.h"
#include "user/SceneList.h"
#include "../GameObject/GameObjectManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize()
{
	// シーン初期化
	currentScene_ = new TitleScene(); // タイトルシーン生成
	currentScene_->commonInitialize(); // 共通初期化を行う
	currentScene_->Initialize(); // 初期化を行う
}

void SceneManager::Update()
{
	// 現在のシーンから次のシーンへ遷移するよう指示されたら
	if (currentScene_->nextScene_ != nullptr) {
		GameObjectManager::GetInstance()->Initialize();
		// シーン初期化
		currentScene_ = currentScene_->nextScene_; // 次のシーンを取得
		currentScene_->commonInitialize(); // 取得したシーンの共通初期化を行う
		currentScene_->Initialize(); // 取得したシーンの初期化を行う
	}

	// 現在のシーンの更新
	currentScene_->Update();

}

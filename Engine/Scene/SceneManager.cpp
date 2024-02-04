#include "SceneManager.h"
#include "SceneList.h"
#include "../GameObject/GameObjectManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Init()
{
	// フェード演出マネージャ初期化
	fadeManager_ = FadeManager::GetInstance();
	fadeManager_->Init();

	// シーン初期化
	currentScene_ = new GameScene(); // タイトルシーン生成
#ifdef _DEBUG // デバッグ時のみサンプルシーンを生成
	// シーン初期化
	//currentScene_ = new SampleScene(); // タイトルシーン生成
#endif // _DEBUG // デバッグ時のみサンプルシーンを生成
	currentScene_->PreInitialize(); // 共通初期化を行う
	currentScene_->Init(); // 初期化を行う
}

void SceneManager::Update()
{
#ifdef _DEBUG

	// FPSカウンターの表示
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

#endif // _DEBUG

	// 現在のシーンから次のシーンへ遷移するよう指示されたら
	if (nextScene_ != nullptr) {
		GameObjectManager::GetInstance()->Init();
		ParticleEmitterManager::GetInstance()->Init();
		AnimationManager::GetInstance()->Init();
		// 現在のシーンがあるなら
		if (currentScene_) {
			// 現在のシーンを削除
			delete currentScene_;
		}
		// シーン初期化
		currentScene_ = nextScene_; // 次のシーンを取得
		nextScene_ = nullptr; // 次のシーンにNULLを代入
		currentScene_->PreInitialize(); // 取得したシーンの共通初期化を行う
		currentScene_->Init(); // 取得したシーンの初期化を行う
	}

	// 現在のシーンの更新
	currentScene_->Update();

	// フェード演出マネージャ更新
	fadeManager_->Update();

#ifdef _DEBUG // デバッグ時のみImGuiの表示

	// ImGuiの表示関数
	fadeManager_->DisplayImGui();

#endif // _DEBUG

}
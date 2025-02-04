#include "SceneManager.h"
#include "App/Scene/SceneList.h"
#include "Engine/GameObject/GameObjectManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Init()
{
	// シーン初期化
	currentScene_ = sceneFactory_->CreateScene("Game"); // ゲームシーン生成
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
		GameObjectManager::GetInstance()->Update(); // ゲームオブジェクトマネージャーの更新
	}

	// 現在のシーンの更新
	currentScene_->Update();
}

void SceneManager::DisplayImGui()
{
	// 現在シーンのImGuiを表示
	currentScene_->DisplayImGui();
}

void SceneManager::ChangeScene(const std::string& name)
{
	// 次のシーンに新しいシーンを代入する
	nextScene_ = std::move(sceneFactory_->CreateScene(name));
}

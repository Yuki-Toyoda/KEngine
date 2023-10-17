#include "StageManager.h"
#include "user/StageList.h"
#include "../GameObject/GameObjectManager.h"

StageManager* StageManager::GetInstance()
{
	static StageManager sceneManager;
	return &sceneManager;
}

void StageManager::Initialize()
{
	// ステージ初期化
	currentStage_ = nullptr;

	for (size_t i = 0; i < kMaxItem_; i++)
	{
		items_.emplace_back(new Item);
		items_.back()->Initialize("Item", BaseObject::tagItem);
		items_.back()->SetIsActive(false);
	}

}

void StageManager::Update()
{
#ifdef _DEBUG

	// FPSカウンターの表示
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

#endif // _DEBUG

	// 現在のシーンから次のシーンへ遷移するよう指示されたら
	if (currentStage_->nextStage_ != nullptr) {
		GameObjectManager::GetInstance()->Initialize();
		// シーン初期化
		currentStage_ = currentStage_->nextStage_; // 次のステージを取得
		currentStage_->commonInitialize(); // 取得したステージの共通初期化を行う
		currentStage_->Initialize(); // 取得したステージの初期化を行う
	}

	// 現在のシーンの更新
	currentStage_->Update();

}

void StageManager::LoadStage()
{

}
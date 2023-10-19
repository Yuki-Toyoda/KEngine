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
	globalVariables_ = GlobalVariables::GetInstance();

	// ステージ初期化
	currentStage_ = nullptr;

	items_.clear();
	items_.resize(kMaxItem_);
	for (size_t i = 0; i < items_.size(); i++)
	{
		items_.push_back(new Item);
		items_.back()->Initialize("Item", BaseObject::tagItem);
		items_.back()->SetIsActive(false);
	}

}

void StageManager::Reset()
{
	for (size_t i = 0; i < kMaxItem_; i++)
	{
		items_.back()->Initialize("Item", BaseObject::tagItem);
		items_.back()->SetIsActive(false);
	}
}

void StageManager::Update()
{
#ifdef _DEBUG

	//// FPSカウンターの表示
	//ImGui::Begin("Control panel");
	//ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	//ImGui::End();

#endif // _DEBUG

	// 現在のシーンの更新
	currentStage_->Update();

}

void StageManager::SetStage(BaseStage* stage)
{
	currentStage_ = stage;
	currentStage_->Initialize(infos_.back());
}

void StageManager::LoadStages()
{
	// 作ったステージの個数を取得
	kMaxStaegNum_ = globalVariables_->GetIntValue("Stage", "kMaxStageNum");
	// ステージの回数分読み込む
	infos_.resize(kMaxStaegNum_);
	for (size_t i = 0; i < infos_.size(); i++)
	{
		// Item の位置、リポップフラグ
		// クリアゲージ
		// 敵
		// カタパルトの個数、位置

	}

}

void StageManager::LoadStageInfo()
{

}

#include "StageManager.h"
#include "../GameObjectManager.h"
#include "../../GlobalVariables/GlobalVariables.h"

void StageManager::Initialize()
{
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// 調整項目クラスのインスタンス取得
	globalVariables_ = GlobalVariables::GetInstance();
	
	// 床のリストをクリア
	stageFloorList_.clear();

	// 床個数リセット
	floorCount_ = globalVariables_->GetIntValue("StageManager", "FloorCount");

	for (int i = 0; i < floorCount_; i++) {
		// 床を追加
		AddStageFloor({ 0.0f, 0.0f, 0.0f }, { 25.0f, 0.25f, 25.0f });
	}

	// ImGui用変数リセット
	AddFloorTranslate_ = {0.0f, 0.0f, 0.0f}; // 追加する床の座標
	AddFloorScale_ = {1.0f, 1.0f, 1.0f}; // 追加する床の大きさ
}

void StageManager::Update()
{
	// 全ての床の座標をjsonファイルにて調整
	for (StageFloor* floor : stageFloorList_) {
		// 調整項目クラスで調整した値を適用する
		floor->transform_.translate_ = globalVariables_->GetVector3Value("StageManager", floor->GetObjectName() + "translate");
		floor->transform_.scale_ = globalVariables_->GetVector3Value("StageManager", floor->GetObjectName() + "scale");
	}

#ifdef _DEBUG
	// ImGui開始
	ImGui::Begin("StageManager");
	ImGui::DragFloat3("translate", &AddFloorTranslate_.x, 0.5f);
	ImGui::DragFloat3("scale", &AddFloorScale_.x, 0.5f);
	if (ImGui::Button("AddStageFloor")) {
		// 床を生成
		AddStageFloor(AddFloorTranslate_, AddFloorScale_);
		floorCount_++;
		globalVariables_->SetValue("StageManager", "FloorCount", floorCount_); // 調整項目クラスに値を追加
	}
	ImGui::End();
#endif // _DEBUG
}

void StageManager::AddStageFloor(Vector3 translate, Vector3 scale)
{
	// 新しい床のインスタンス生成
	StageFloor* floor = new StageFloor(); // インスタンス生成
	floor->Initialize("StageFloor", BaseObject::Floor); // 初期化
	gameObjectManager_->AddGameObject(floor); // 生成したインスタンスをマネージャーに追加
	stageFloorList_.push_back(floor); // ステージの床リストにインスタンスを追加
	globalVariables_->AddItem("StageManager", floor->GetObjectName() + "translate", floor->transform_.translate_); // 調整項目クラスに値を追加
	globalVariables_->AddItem("StageManager", floor->GetObjectName() + "scale", floor->transform_.scale_); // 調整項目クラスに値を追加
}

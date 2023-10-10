#include "StageManager.h"
#include "../GameObjectManager.h"
#include "../../GlobalVariables/GlobalVariables.h"

void StageManager::Initialize()
{
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// 調整項目クラスのインスタンス取得
	globalVariables_ = GlobalVariables::GetInstance();
	
	// とりあえず初期設定
	goalPos_ = { 0.0f, 1.5f, 30.0f };

	// ゴール生成
	goal_ = std::make_unique<Goal>(); // インスタンス生成
	goal_->Initialize("goal", BaseObject::StageGoal); // 初期化
	goal_->transform_.translate_ = goalPos_; // 座標初期化
	gameObjectManager_->AddGameObject(goal_.get()); // リストに追加

	// 床のリストをクリア
	stageFloorList_.clear();

	// 床個数リセット
	floorCount_ = globalVariables_->GetIntValue("StageManager", "FloorCount");

	for (int i = 0; i < floorCount_; i++) {
		// 床を追加
		AddStageFloor({ 0.0f, 0.0f, 0.0f }, { 25.0f, 0.25f, 25.0f });
	}

	moveFloor1_ = std::make_unique<StageMoveFloor>(); // インスタンス生成
	moveFloor1_->Initialize("moveFloor", BaseObject::MoveFloor); // 初期化
	gameObjectManager_->AddGameObject(moveFloor1_.get()); // リストに追加

	// ImGui用変数リセット
	AddFloorTranslate_ = {0.0f, 0.0f, 0.0f}; // 追加する床の座標
	AddFloorScale_ = {1.0f, 1.0f, 1.0f}; // 追加する床の大きさ
}

void StageManager::Update()
{

	// ステージ床リストをクリア
	stageFloorList_.clear();

	// オブジェクトマネージャーからステージの床を取得
	stageFloorList_ = gameObjectManager_->GetGameObject(BaseObject::Floor);

	// ゴールの座標取得
	goal_->transform_.translate_ = globalVariables_->GetVector3Value("StageManager", goal_->GetObjectName() + "translate");

	// 床の数を取得
	floorCount_ = (int)stageFloorList_.size();
	globalVariables_->SetValue("StageManager", "FloorCount", floorCount_); // 調整項目クラスに値を追加

	// 全ての床の座標をjsonファイルにて調整
	for (BaseObject* floor : stageFloorList_) {
		// 調整項目クラスで調整した値を適用する
		Vector3 test = globalVariables_->GetVector3Value("StageManager", floor->GetObjectName() + "translate");
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
	}

	// 床が2個以上ある場合
	if (floorCount_ > 1) {
		if (ImGui::Button("DeleteFloor")) {
			std::string objectName = "StageFloor" + std::to_string(floorCount_);
			gameObjectManager_->DeleteGameObject(objectName);
		}
	}

	ImGui::End();
#endif // _DEBUG
}

void StageManager::AddStageFloor(Vector3 translate, Vector3 scale)
{
	// 新しい床のインスタンス生成
	StageFloor* floor = new StageFloor(); // インスタンス生成
	floor->Initialize("StageFloor", BaseObject::Floor); // 初期化
	floor->transform_.translate_ = translate; // 座標設定
	floor->transform_.scale_ = scale; // 座標設定
	gameObjectManager_->AddGameObject(floor); // 生成したインスタンスをマネージャーに追加
	globalVariables_->AddItem("StageManager", floor->GetObjectName() + "translate", floor->transform_.translate_); // 調整項目クラスに値を追加
	globalVariables_->AddItem("StageManager", floor->GetObjectName() + "scale", floor->transform_.scale_); // 調整項目クラスに値を追加
}

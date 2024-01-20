#include "GameEditor.h"
#include "../../../Base/WinApp.h"
#include "../../../Math/Math.h"

void GameEditor::Init()
{
	dataManager_ = GameDataManager::GetInstance();
	gameObjectManager_ = GameObjectManager::GetInstance();

	CreateCamera();

	ParameterInitialize();

	dataManager_->CreateGroup({ "MeteorParam","SingleAttack" });
	dataManager_->SaveData("MeteorParam");

}

void GameEditor::Update()
{
	// エディターのImGui表示
	ImGuiUpdate();
}

void GameEditor::ParameterInitialize()
{
	// 構造の名前取得
	HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[0] };

	names = { "Boss","A" };
	dataManager_->AddItem(names, dataManager_->kObstacleItems[2], kMaxMeteor_);
	dataManager_->AddItem({"MeteorParam","A"}, "IsGravity", isGravity_);
	kMaxMeteor_ = dataManager_->GetIntValue(names, "MaxCount");
	isGravity_ = dataManager_->GetIntValue({ "MeteorParam","A" }, "IsGravity");

	for (int i = 0; i < kMaxMeteor_; i++) {
		names.kSection = "Meteor";
		Meteor* meteor;
		meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);

		// パス生成
		std::string section = names.kSection + std::to_string(i);
		std::string key = "Position";

		// ここから情報を受け取る部分
		Vector3 newPosition = {};
		// 要素がなかった場合の処理
		dataManager_->AddItem({ names.kGroup,section }, key, newPosition);
		newPosition = dataManager_->GetVector3Value({ names.kGroup,section }, key);
		meteor->transform_.translate_ = newPosition;
		// プッシュ
		meteors_.push_back(meteor);

	}

}

void GameEditor::ImGuiUpdate()
{
	//GetMouseCursor();

	ImGui::Begin("Editor");

//	if (ImGui::Button("Save")) {
//		// 名前
//		HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_],dataManager_->kObjectNames[0] };
//
//#pragma region 保存処理
//		// 最大数の設定
//		dataManager_->SetValue(names, dataManager_->kEnemyItems[4], kMaxEnemyCount_);
//		Vector3 testValue = {};
//		// 数初期化
//		indexCount_ = 0;
//		// 敵
//		for (IEnemy* enemy : enemies_) {
//			// 保存処理
//			SaveObject(enemy, 0);
//			// カウント
//			indexCount_++;
//		}
//
//		indexCount_ = 0;
//		// 最大数の設定
//		names.kSection = dataManager_->kObjectNames[1];
//		dataManager_->SetValue(names, dataManager_->kObstacleItems[2], kMaxObstacleCount_);
//
//		// 障害物
//		for (Obstacle* obstacle : obstacles_) {
//			// 保存処理
//			SaveObject(obstacle, 1);
//			// カウント
//			indexCount_++;
//
//		}
//#pragma endregion
//		// セーブ
//		dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
//		// メッセージボックス表示
//		std::string message = std::format("{}.json saved.", "StageData");
//		MessageBoxA(nullptr, message.c_str(), "Editors", 0);
//
//	}

	if (ImGui::Button("Reload")) {
		DataReaload();
	}

	if (ImGui::Button("BossSave")) {
		// 名前
		HierarchicalName names = { "Boss","A" };

		// 最大数の設定
		dataManager_->SetValue(names, dataManager_->kEnemyItems[4], kMaxMeteor_);
		Vector3 testValue = {};
		// 数初期化
		indexCount_ = 0;
		for (Meteor* meteor : meteors_) {
			SaveObject(meteor, 2);
			ImGui::Text("\n");
			indexCount_++;
		}

		dataManager_->SaveData(names.kGroup);
	}

	ImGui::Text("\n");

	//if (ImGui::CollapsingHeader("StageSelect")) {

	//	ImGui::Bullet();
	//	if (ImGui::SmallButton("Easy")) {
	//		editNowLevel_ = 0;
	//		// 保存処理
	//		dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
	//		// リロード
	//		DataReaload();
	//	}

	//	ImGui::Bullet();
	//	if (ImGui::SmallButton("Normal")) {
	//		editNowLevel_ = 1;
	//		// 保存処理
	//		dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
	//		// リロード
	//		DataReaload();
	//	}

	//	ImGui::Bullet();
	//	if (ImGui::SmallButton("Hard")) {
	//		editNowLevel_ = 2;
	//		// 保存処理
	//		dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
	//		// リロード
	//		DataReaload();
	//	}

	//	ImGui::Bullet();
	//	if (ImGui::SmallButton("Boss")) {
	//		// 保存処理
	//		dataManager_->SaveData("Boss");
	//		// リロード
	//		DataReaload();
	//	}
	//}

	//ImGui::Text("\n");

	if (ImGui::BeginTabBar("EditObject")) {
		// 追加分のタブ
		if (ImGui::BeginTabItem("Meteor")) {

			static bool gravity = static_cast<bool>(isGravity_);
			
			if (ImGui::Checkbox("Gravity", &gravity)) {
				if (!gravity && isGravity_) {
					DataReaload();
				}

				isGravity_ = static_cast<int>(gravity);
				dataManager_->SetValue({ "MeteorParam","A" }, "IsGravity", isGravity_);
				dataManager_->SaveData("MeteorParam");				
			}


			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300), ImGuiWindowFlags_NoTitleBar);

			for (Meteor* meteor : meteors_) {
				std::string fullTag = meteor->GetObjectName();
				ImGui::DragFloat3(fullTag.c_str(), &meteor->transform_.translate_.x, 0.1f, -30.0f, 30.0f);
				ImGui::Text("\n");
			}
			ImGui::EndChild();

			ImGui::InputInt("MeteorMaxCount", &kMaxMeteor_);
			const int nowMax = 20;
			if (kMaxMeteor_ > nowMax) {
				kMaxMeteor_ = nowMax;
			}
			// 敵の追加
			if (ImGui::Button("Add")) {
				Meteor* meteor;
				meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
				meteors_.push_back(meteor);
				kMaxMeteor_++;
			}


			ImGui::EndTabItem();
		}

		// 障害物のタブ
		if (ImGui::BeginTabItem("Obstacle")) {
			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300), ImGuiWindowFlags_NoTitleBar);

			float paramAbs = 30.0f;
			for (Obstacle* obstacle : obstacles_) {
				std::string fullTag = obstacle->GetObjectName();
				ImGui::DragFloat3(fullTag.c_str(), &obstacle->transform_.translate_.x, 0.1f, -paramAbs, paramAbs);
				ImGui::Text("\n");
			}

			ImGui::EndChild();

			ImGui::InputInt("ObstacleMaxCount", &kMaxObstacleCount_);
			// 敵の追加
			if (ImGui::Button("Add")) {
				Obstacle* obstacle;
				obstacle = gameObjectManager_->CreateInstance<Obstacle>("obstacle", BaseObject::TagObstacle);
				obstacles_.push_back(obstacle);
			}


			ImGui::EndTabItem();
		}

		// タブバーを終了
		ImGui::EndTabBar();
	}

	ImGui::End();

}

void GameEditor::DataReaload()
{
	// Jsonからの情報を再取得
	GameDataManager::GetInstance()->ClearGroup();
	GameDataManager::GetInstance()->LoadFiles();
	// オブジェクト内容初期化
	gameObjectManager_->Init();
	// カメラ設定
	CreateCamera();
	// リストクリア
	enemies_.clear();
	obstacles_.clear();
	meteors_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

void GameEditor::CreateCamera()
{

	//ground_ = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	//ground_->transform_.scale_ = { 55.81f,1.0f,32.5f };
	
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	//camera_->transform_.translate_ = { 0.0f,47.0f,-85.0f };
	camera_->transform_.translate_ = { 0,100.0f,0 };
	camera_->transform_.rotate_ = { 1.57f,0.0f,0.0f };

	//Ground* ground;
	//ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	//ground->transform_.scale_ = { 55.81f,1.0f,32.5f };
}

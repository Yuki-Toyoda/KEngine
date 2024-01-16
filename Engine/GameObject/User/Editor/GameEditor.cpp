#include "GameEditor.h"
#include "../../../Base/WinApp.h"
#include "../../../Math/Math.h"

void GameEditor::Init()
{
	dataManager_ = GameDataManager::GetInstance();
	gameObjectManager_ = GameObjectManager::GetInstance();

	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;

	ParameterInitialize();

}

void GameEditor::Update()
{
	ImGuiProgress();
}

void GameEditor::ParameterInitialize()
{
	// 構造の名前取得
	HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[0] };
#pragma region 敵の初期化
	// 最大値設定
	dataManager_->AddItem(names, dataManager_->kEnemyItems[4], kMaxEnemyCount_);
	kMaxEnemyCount_ = dataManager_->GetIntValue({ dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[0] }, dataManager_->kEnemyItems[4]);
	// 敵の数だけ
	for (int i = 0; i < kMaxEnemyCount_; i++) {
		Enemy* enemy;
		// オブジェクトマネージャーに追加
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		// パスの設定
		std::string sectionPath = dataManager_->kObjectNames[0] + std::to_string(i);
		std::string keyPath = dataManager_->kEnemyItems[0];
		// 座標設定
		Vector3 newPos = {};
		dataManager_->AddItem({ names.kGroup,sectionPath }, keyPath, newPos);
		newPos = dataManager_->GetVector3Value({ names.kGroup,sectionPath }, keyPath);
		enemy->transform_.translate_ = newPos;
		// リスト追加
		enemies_.push_back(enemy);
	}
#pragma endregion

#pragma region 障害物の初期化
	names = { dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[1] };
	// 最大値設定
	dataManager_->AddItem(names, dataManager_->kObstacleItems[2], kMaxObstacleCount_);
	kMaxObstacleCount_ = dataManager_->GetIntValue(names, dataManager_->kObstacleItems[2]);
	// 障害物の数だけ
	for (int i = 0; i < kMaxObstacleCount_; i++) {
		Obstacle* obstacle;
		// オブジェクトマネージャーに追加
		obstacle = gameObjectManager_->CreateInstance<Obstacle>("Obstacle", BaseObject::TagObstacle);
		// パスの設定
		std::string sectionPath = dataManager_->kObjectNames[1] + std::to_string(i);
		std::string keyPath = dataManager_->kObstacleItems[0];
		// 座標設定
		Vector3 newPos = {};
		dataManager_->AddItem({ names.kGroup,sectionPath }, keyPath, newPos);
		newPos = dataManager_->GetVector3Value({ names.kGroup,sectionPath }, keyPath);
		obstacle->transform_.translate_ = newPos;
		// リスト追加
		obstacles_.push_back(obstacle);
	}
#pragma endregion

}

void GameEditor::ImGuiProgress()
{
	//GetMouseCursor();

	ImGui::Begin("Editor");

	if (ImGui::Button("Save")) {
		// 名前
		HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_],dataManager_->kObjectNames[0] };

#pragma region 保存処理
		// 最大数の設定
		dataManager_->SetValue(names, dataManager_->kEnemyItems[4], kMaxEnemyCount_);
		Vector3 testValue = {};
		// 数初期化
		indexCount_ = 0;
		// 敵の保存処理
		for (Enemy* enemy : enemies_) {
			// セクションパス
			std::string sectionPath = dataManager_->kObjectNames[0] + std::to_string(indexCount_);
			// アイテムパス
			std::string keyPath = dataManager_->kEnemyItems[0];
			// 座標設定
			dataManager_->SetValue({ names.kGroup,sectionPath }, keyPath, enemy->transform_.GetWorldPos());
			// カウント
			indexCount_++;
		}

		indexCount_ = 0;
		// 最大数の設定
		names.kSection = dataManager_->kObjectNames[1];
		dataManager_->SetValue(names, dataManager_->kObstacleItems[2], kMaxObstacleCount_);

		// 障害物の保存処理
		for (Obstacle* obstacle : obstacles_) {
			// セクションパス
			std::string sectionPath = dataManager_->kObjectNames[1] + std::to_string(indexCount_);
			// アイテムパス
			std::string keyPath = dataManager_->kObstacleItems[0];
			// 座標設定
			dataManager_->SetValue({ names.kGroup,sectionPath }, keyPath, obstacle->transform_.GetWorldPos());
			// カウント
			indexCount_++;

		}
#pragma endregion
		// セーブ
		dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
		// メッセージボックス表示
		std::string message = std::format("{}.json saved.", "StageData");
		MessageBoxA(nullptr, message.c_str(), "Editors", 0);

	}

	if (ImGui::Button("Reload")) {
		DataReaload();
	}

	ImGui::Text("\n");

	if (ImGui::CollapsingHeader("StageSelect")) {

		if (ImGui::Button("Easy")) {
			editNowLevel_ = 0;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
		if (ImGui::Button("Normal")) {
			editNowLevel_ = 1;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
		if (ImGui::Button("Hard")) {
			editNowLevel_ = 2;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
	}

	ImGui::Text("\n");

	if (ImGui::BeginTabBar("EditObject")) {
		// 敵のタブ
		if (ImGui::BeginTabItem("Enemy")) {
			// タブの内容
			ImGui::Text("\n");
			ImGui::InputInt("EnemyMaxNumber", &kMaxEnemyCount_);
			ImGui::Text("\n");

			// 敵の追加
			if (ImGui::Button("Add")) {
				Enemy* enemy;
				enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
				enemies_.push_back(enemy);
			}

			// 座標の変更
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300), ImGuiWindowFlags_NoTitleBar);

			float paramAbs = 30.0f;
			for (Enemy* enemy : enemies_) {
				std::string fullTag = enemy->GetObjectName();
				ImGui::DragFloat3(fullTag.c_str(), &enemy->transform_.translate_.x, 0.1f, -paramAbs, paramAbs);
				ImGui::Text("\n");
			}

			ImGui::EndChild();

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

		// 追加分のタブ
		if (ImGui::BeginTabItem("Tab 2")) {
			// タブの内容
			ImGui::Text("This is tab 2!");
			ImGui::EndTabItem();
		}
		// タブバーを終了
		ImGui::EndTabBar();
	}

	ImGui::End();

}

void GameEditor::GetMouseCursor()
{
	POINT mousePos;
	// マウスの座標取得
	GetCursorPos(&mousePos);
	// スクリーンサイズに合わせる
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	// マウスの座標をスクリーンに合わせる
	ScreenToClient(hwnd, &mousePos);

	Vector3 nowPoint_ = { (float)mousePos.x,(float)mousePos.y,0 };
	mouseV2Position_ = { (float)mousePos.x,(float)mousePos.y };

	// ビューポート行列
	Matrix4x4 matViewport = Math::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kwindowHeight, 0, 1);
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = camera_->GetViewProjectionMatrix() * matViewport;
	
	// 合成行列の逆行列
	Matrix4x4 matInverseVPV = Math::Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(nowPoint_.x, nowPoint_.y, 0);
	Vector3 posFar = Vector3(nowPoint_.x, nowPoint_.y, 1);

	// スクリーン座標系からワールド座標系へ
	//posNear = Math::Transform(posNear, matInverseVPV);
	//posFar = Math::Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Math::Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	float distance = 50.0f;
	mouseDirection.x *= distance;
	mouseDirection.y *= distance;
	mouseDirection.z = 0;

	mouseV3Position_ = mouseDirection;
	//return mouseDirection;
}

void GameEditor::DataReaload()
{
	// Jsonからの情報を再取得
	GameDataManager::GetInstance()->ClearGroup();
	GameDataManager::GetInstance()->LoadFiles();
	// オブジェクト内容初期化
	gameObjectManager_->Init();
	// カメラ設定
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	// リストクリア
	enemies_.clear();
	obstacles_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

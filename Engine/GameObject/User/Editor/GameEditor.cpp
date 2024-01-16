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
	HierarchicalName names = { dataManager_->kLevelNames[0], dataManager_->kObjectNames[0] };
	dataManager_->AddItem(names, dataManager_->kItemNames[4], kMaxEnemyCount_);

	// 最大値設定
	kMaxEnemyCount_ = dataManager_->GetIntValue({ dataManager_->kLevelNames[0], dataManager_->kObjectNames[0] }, dataManager_->kItemNames[4]);

	for (int i = 0; i < kMaxEnemyCount_; i++) {
		Enemy* enemy;
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		std::string sectionPath = dataManager_->kObjectNames[0] + std::to_string(i);
		std::string keyPath = dataManager_->kItemNames[0];
		Vector3 newPos = dataManager_->GetVector3Value({ names.kGroup,sectionPath }, keyPath);
		enemy->SetPosition(newPos);
		enemies_.push_back(enemy);
	}
}

void GameEditor::ImGuiProgress()
{

	//mousePosition_ = GetMouseCursor();
	this->GetMouseCursor();

	ImGui::Begin("Editor");

	ImGui::DragFloat2("MouseV2", &mouseV2Position_.x);
	ImGui::DragFloat3("MouseV3", &mouseV3Position_.x);

	if (ImGui::Button("Easy")) {
		editPhaseNum_++;
	}
	if (ImGui::Button("Normal")) {
		if (editPhaseNum_ > 0) {
			editPhaseNum_--;
		}
	}
	if (ImGui::Button("Hard")) {

	}
	if (ImGui::Button("Reload")) {
		DataReaload();
	}

	ImGui::Text("\n");

	if (ImGui::BeginTabBar("EditObject")) {
		// タブアイテム 0
		if (ImGui::BeginTabItem("Enemy")) {
			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300), ImGuiWindowFlags_NoTitleBar);

			float paramAbs = 30.0f;
			for (Enemy* enemy : enemies_) {
				std::string fullTag = enemy->GetObjectName();
				ImGui::DragFloat3(fullTag.c_str(), &enemy->transform_.translate_.x, 0.1f, -paramAbs, paramAbs);
				ImGui::Text("\n");
			}

			ImGui::EndChild();

			ImGui::InputInt("EnemyMaxNumber", &kMaxEnemyCount_);
			// 敵の追加
			if (ImGui::Button("Add")) {
				Enemy* enemy;
				enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
				enemies_.push_back(enemy);
			}
			if (ImGui::Button("Delete")) {
				enemies_.pop_back();
			}
			if (ImGui::Button("Save")) {
				// 名前
				HierarchicalName names = { dataManager_->kLevelNames[0],dataManager_->kObjectNames[0] };

				// 最大数の設定
				dataManager_->SetValue(names, dataManager_->kItemNames[4], kMaxEnemyCount_);
				Vector3 testValue = {};
				// 数初期化
				enemyCount_ = 0;
				// 敵の数分
				for (Enemy* enemy : enemies_) {
					// セクションパス
					std::string sectionPath = dataManager_->kObjectNames[0] + std::to_string(enemyCount_);
					// アイテムパス
					std::string keyPath = dataManager_->kItemNames[0];

					// 座標設定
					dataManager_->SetValue({ names.kGroup,sectionPath }, keyPath, enemy->transform_.GetWorldPos());
					dataManager_->SetValue({ names.kGroup,sectionPath }, "TestValue", testValue);
					// セーブ
					dataManager_->SaveData(dataManager_->kLevelNames[0]);
					// カウント
					enemyCount_++;
				}
				std::string message = std::format("{}.json saved.", "StageData");
				MessageBoxA(nullptr, message.c_str(), "Editors", 0);

			}
			ImGui::EndTabItem();
		}

		// タブアイテム 1
		if (ImGui::BeginTabItem("Block")) {
			// タブの内容
			ImGui::Text("This is tab 1!");
			ImGui::EndTabItem();
		}

		// タブアイテム 2
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
	enemies_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

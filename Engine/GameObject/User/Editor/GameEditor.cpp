#include "GameEditor.h"
#include "../../../Base/WinApp.h"
#include "../../../Math/Math.h"

void GameEditor::Init()
{
	dataManager_ = GameDataManager::GetInstance();
	gameObjectManager_ = GameObjectManager::GetInstance();

	CreateCamera();

	ParameterInitialize();
	HierarchicalName names = { "MeteorParam","Multi" };
	dataManager_->CreateGroup(names);
	dataManager_->AddItem(names, "CoolTime", coolTime_);
	dataManager_->AddItem(names, "MaxCount", kMaxCount_);
	dataManager_->SaveData(names.kGroup);
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

#pragma region 共通のパラメータ
	// 名前
	names = GetNormalInfo();
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "IsGravity", isGravity_);
	dataManager_->AddItem(names, "Scale", size_);

	// Jsonから受け取る
	kMaxMeteor_ = dataManager_->GetIntValue(names, "MaxCount");
	isGravity_ = dataManager_->GetIntValue(names, "IsGravity");
	size_ = dataManager_->GetVector3Value(names, "Scale");


	names.kSection = "Multi";
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "CoolTime", coolTime_);
	dataManager_->AddItem(names, "MaxCount", kMaxCount_);
	// Jsonから受け取る
	coolTime_ = dataManager_->GetFloatValue(names, "CoolTime");
	kMaxCount_ = dataManager_->GetIntValue(names, "MaxCount");

#pragma endregion

#pragma region 通常落下部分
	// 名前
	names = GetSingleInfo();
	// 隕石ごとに
	for (int i = 0; i < kMaxMeteor_; i++) {
		Meteor* meteor;
		meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);

		// パス生成
		std::string group = names.kGroup;
		std::string section = "Meteor" + std::to_string(i);
		std::string key = "Position";

		// ここから情報を受け取る部分
		Vector3 newPosition = {};
		// 要素がなかった場合の処理
		dataManager_->AddItem({ group,section }, key, newPosition);
		newPosition = dataManager_->GetVector3Value({ group,section }, key);
		meteor->transform_.translate_ = newPosition;
		meteor->transform_.translate_.y = 0.8f;
		// プッシュ
		meteors_.push_back(meteor);

	}
#pragma endregion

}

void GameEditor::ImGuiUpdate()
{
	indexCount_ = 0;

	ImGui::Begin("Editor");

	// ボス用の保存ボタン
	if (ImGui::Button("BossSave")) {
		// 名前
		HierarchicalName names;

		SaveInfoParameter();

		Vector3 testValue = {};
		// 数初期化
		indexCount_ = 0;
		for (Meteor* meteor : meteors_) {
			SaveObject(meteor, 2);
			ImGui::Text("\n");
			indexCount_++;
		}

		// 隕石のファイル
		dataManager_->SaveData("SingleMeteor");
		// 共通のパラメータファイル
		dataManager_->SaveData("MeteorParam");
		// セーブが完了したことをメッセージボックスで出す
		std::string message = std::format("{} saved.", "Data");
		MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
	}
	// 更新
	if (ImGui::Button("Reload")) {
		DataReaload();
	}

	ImGui::Text("\n");

	if (ImGui::CollapsingHeader("StageSelect")) {
		ImGui::Bullet();
		if (ImGui::SmallButton("Easy")) {
			editNowLevel_ = 0;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
		ImGui::Bullet();
		if (ImGui::SmallButton("Normal")) {
			editNowLevel_ = 1;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
		ImGui::Bullet();
		if (ImGui::SmallButton("Hard")) {
			editNowLevel_ = 2;
			// 保存処理
			dataManager_->SaveData(dataManager_->kLevelNames[editNowLevel_]);
			// リロード
			DataReaload();
		}
		ImGui::Bullet();
		if (ImGui::SmallButton("Boss")) {
			// 保存処理
			std::string fileName = "Boss" + std::to_string(GameDataManager::LoadNumber);
			dataManager_->SaveData("Boss");
			// リロード
			DataReaload();
		}
	}
	ImGui::Text("\n");

	if (ImGui::BeginTabBar("EditObject")) {
		// 追加分のタブ
		if (ImGui::BeginTabItem("SingleAttack")) {

			// 落下
			static bool gravity = static_cast<bool>(isGravity_);
			if (ImGui::Checkbox("Gravity", &gravity)) {
				if (!gravity && isGravity_) {
					DataReaload();
				}

				isGravity_ = static_cast<int>(gravity);
				dataManager_->SetValue(GetNormalInfo(), "IsGravity", isGravity_);
				dataManager_->SaveData("MeteorParam");				
			}
			// サイズ
			ImGui::DragFloat3("Meteor_Scale", &size_.x, 0.01f, 1.0f, 20.0f);

			ImGui::Text("\n");

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 300), ImGuiWindowFlags_NoTitleBar);

			for (Meteor* meteor : meteors_) {
				// 名前のタグ
				std::string fullTag = "Meteor" + std::to_string(indexCount_);
				meteor->transform_.scale_ = size_;
				ImGui::DragFloat3(fullTag.c_str(), &meteor->transform_.translate_.x, 0.1f, -30.0f, 30.0f);
				ImGui::Text("\n");
				indexCount_++;

				if (meteor->transform_.translate_.y < 1.0f) {
					meteor->transform_.translate_.y = 1.0f;
				}

			}
			ImGui::EndChild();

			ImGui::InputInt("MeteorMaxCount", &kMaxMeteor_);
			const int nowMax = 20;
			// 敵の追加
			if (ImGui::Button("Add")) {
				// カウントアップ
				kMaxMeteor_++;
				// 最大を超えないように
				if (kMaxMeteor_ > nowMax) {
					kMaxMeteor_ = nowMax;
					ImGui::EndTabItem();
					ImGui::EndTabBar();
					ImGui::End();
					return;
				}
				// インスタンス生成
				Meteor* meteor;
				meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
				std::string group = "SingleMeteor";
				std::string section = "Meteor" + std::to_string(kMaxMeteor_);
				meteor->transform_.translate_ = dataManager_->GetVector3Value({ group,section }, "Position");
				meteors_.push_back(meteor);
			}
			// 最大を超えないように
			if (kMaxMeteor_ > nowMax) {
				kMaxMeteor_ = nowMax;
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("MultiAttack")) {
			ImGui::InputInt("MaxCount", &this->kMaxCount_);
			ImGui::InputFloat("AttackInterval", &coolTime_);

			if (ImGui::Button("Save")) {
				HierarchicalName names = { "MeteorParam","Multi" };
				// 値の設定
				SaveInfoParameter();
				// 書き込み
				dataManager_->SaveData(names.kGroup);
				// セーブが完了したことをメッセージボックスで出す
				std::string message = std::format("{}.json saved.", names.kGroup);
				MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
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
	meteors_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

void GameEditor::CreateCamera()
{

	//Boss* boss;
	//boss = gameObjectManager_->CreateInstance<Boss>("Boss", BaseObject::TagEnemy);
	//boss->SetPlayer(nullptr);

	// カメラ初期化
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	//camera_->transform_.translate_ = { 0.0f,47.0f,-85.0f };
	camera_->transform_.translate_ = { 0,100.0f,0 };
	camera_->transform_.rotate_ = { 1.57f,0.0f,0.0f };

	ground_ = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagMeteor);
	ground_->transform_.scale_ = { 55.81f,1.0f,32.5f };
}

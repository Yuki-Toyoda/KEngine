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
	// 落下修正
	MeteorFix();
	// エディター以外の基本部分のImGui
	SystemImGui();
	// エディターのImGui表示
	EditorImGui();

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
	dataManager_->AddItem(names, "Distance", respawnDistance_);

	// Jsonから受け取る
	kMaxMeteor_ = dataManager_->GetValue<int>(names, "MaxCount");
	isGravity_ = dataManager_->GetValue<int>(names, "IsGravity");
	size_ = dataManager_->GetValue<Vector3>(names, "Scale");
	respawnDistance_ = dataManager_->GetValue<float>(names, "Distance");

	// 名前
	names.kSection = "Multi";
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "CoolTime", coolTime_);
	dataManager_->AddItem(names, "MaxCount", kMaxCount_);

	// Jsonから受け取る
	coolTime_ = dataManager_->GetValue<float>(names, "CoolTime");
	kMaxCount_ = dataManager_->GetValue<int>(names, "MaxCount");

	names.kSection = "PushUp";
	// パラメータない用
	dataManager_->AddItem(names, "MaxCount", kMaxPushUp_);

	kMaxPushUp_ = dataManager_->GetValue<int>(names, "MaxCount");

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
		newPosition = dataManager_->GetValue<Vector3>({ group,section }, key);

		meteor->transform_.translate_ = newPosition;
		meteor->transform_.translate_.y = 0.8f;
		// プッシュ
		meteors_.push_back(meteor);

	}

#pragma endregion

#pragma region 下からの攻撃

	names = { "PushUpAttack","Info" };

	for (int i = 0; i < kMaxPushUp_; i++) {
		PushUp* object;
		object = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);

		// パス生成
		std::string group = "PushUpAttack";
		std::string section = "PushUp" + std::to_string(i);
		std::string key = "Position";

		// ここから情報を受け取る部分
		Vector3 newPosition = {};
		// 要素がなかった場合の処理
		dataManager_->AddItem({ group,section }, key, newPosition);
		newPosition = dataManager_->GetValue<Vector3>({ group,section }, key);

		object->transform_.translate_ = newPosition;
		object->transform_.translate_.y = 0.8f;
		// プッシュ
		pushUps_.push_back(object);
	}

#pragma endregion

}

void GameEditor::EditorImGui()
{
	indexCount_ = 0;

	ImGui::Begin("Editor");
	ImGui::SeparatorText("DataController");
	// ボス用の保存ボタン
	if (ImGui::Button("BossSave")) {
		// 名前
		HierarchicalName names;

		std::string single = "SingleMeteor";
		std::string param = "MeteorParam";

		SaveInfoParameter();

		// 数初期化
		indexCount_ = 0;
		for (Meteor* meteor : meteors_) {
			SetObject(meteor, single, 2);
			ImGui::Text("\n");
			indexCount_++;
		}
		// 名前
		std::string pushUp = "PushUpAttack";
		// 数初期化
		indexCount_ = 0;
		for (PushUp* object : pushUps_) {
			SetObject(object, pushUp, 3);
			ImGui::Text("\n");
			indexCount_++;
		}
		// 突き上げのファイル
		dataManager_->SaveData(pushUp);
		// 隕石のファイル
		dataManager_->SaveData(single);
		// 共通のパラメータファイル
		dataManager_->SaveData(param);
		// セーブが完了したことをメッセージボックスで出す
		std::string message = std::format("{} saved.", "Data");
		MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
	}
	// 更新
	if (ImGui::Button("Reload")) {
		DataReaload();
	}

	ImGui::SeparatorText("Object");

	if (ImGui::BeginTabBar("EditObject")) {
		// 上から落ちてくるやつの方
		if (ImGui::BeginTabItem("SingleAttack")) {
			ImGui::SeparatorText("System");
			if (ImGui::Button("Save")) {
				// 名前
				std::string single = "SingleMeteor";
				// 数初期化
				indexCount_ = 0;
				for (Meteor* meteor : meteors_) {
					SetObject(meteor, single, 2);
					ImGui::Text("\n");
					indexCount_++;
				}
				// 隕石のファイル
				dataManager_->SaveData(single);
				// セーブが完了したことをメッセージボックスで出す
				std::string message = std::format("{}.json saved.", single);
				MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
			}
			ImGui::SeparatorText("Common");
			// 落下
			static bool gravity = static_cast<bool>(isGravity_);
			if (ImGui::Checkbox("Gravity", &gravity)) {
				if (!gravity && isGravity_) {
					DataReaload();
				}

				isGravity_ = static_cast<int>(gravity);
				dataManager_->SetValue(GetNormalInfo(), "IsGravity", isGravity_);
				// パラメータのファイル保存
				dataManager_->SaveData(kParamName);
			}
			// サイズ
			ImGui::DragFloat3("Meteor_Scale", &size_.x, 0.01f, 1.0f, 20.0f);
			// 出現するY座標
			ImGui::DragFloat("Y_Distance", &respawnDistance_, 0.02f, 0, 100.0f);
			// セパレート
			ImGui::SeparatorText("Individual");
			// 最大の数
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
				meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagNone);
				std::string group = "SingleMeteor";
				std::string section = "Meteor" + std::to_string(kMaxMeteor_);
				Vector3 newPos{0,2.0f,0};
				dataManager_->AddItem({ group,section }, "Position", newPos);
				meteor->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				meteors_.push_back(meteor);
			}
			// 最大を超えないように
			if (kMaxMeteor_ > nowMax) {
				kMaxMeteor_ = nowMax;
			}

			ImGui::Text("\n");

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			for (Meteor* meteor : meteors_) {
				// 名前のタグ
				std::string fullTag = "Meteor" + std::to_string(indexCount_);
				// 座標
				ImGui::DragFloat3(fullTag.c_str(), &meteor->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				ImGui::Text("\n");
				indexCount_++;
			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		// 追尾の方
		if (ImGui::BeginTabItem("MultiAttack")) {
			ImGui::SeparatorText("System");
			// 保存
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
			ImGui::SeparatorText("Common");
			// 最大個数
			ImGui::InputInt("MaxCount", &this->kMaxCount_);
			// 間隔
			ImGui::InputFloat("AttackInterval", &coolTime_);
			ImGui::EndTabItem();
		}

		// 下からのやつ
		if (ImGui::BeginTabItem("PushUpAttack")) {
			// 保存
			ImGui::SeparatorText("System");
			if (ImGui::Button("Save")) {
				// 名前
				std::string pushUp = "PushUpAttack";
				// 数初期化
				indexCount_ = 0;
				for (PushUp* object : pushUps_) {
					SetObject(object, pushUp, 3);
					ImGui::Text("\n");
					indexCount_++;
				}
				// 隕石のファイル
				dataManager_->SaveData(pushUp);
				// セーブが完了したことをメッセージボックスで出す
				std::string message = std::format("{}.json saved.", pushUp);
				MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
			}

			// 要素
			ImGui::SeparatorText("Individual");
			// 最大の数
			ImGui::InputInt("PushMaxCount", &kMaxPushUp_);
			const int nowMax = 20;

			// 敵の追加
			if (ImGui::Button("Add")) {
				// カウントアップ
				kMaxPushUp_++;
				// 最大を超えないように
				if (kMaxPushUp_ > nowMax) {
					kMaxPushUp_ = nowMax;
					ImGui::EndTabItem();
					ImGui::EndTabBar();
					ImGui::End();
					return;
				}
				// インスタンス生成
				PushUp* object;
				object = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
				std::string group = "PushUpAttack";
				std::string section = "PushUp" + std::to_string(kMaxPushUp_);
				Vector3 newPos = {};
				dataManager_->AddItem({ group,section }, "Position", newPos);
				object->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				pushUps_.push_back(object);
			}
			// 最大を超えないように
			if (kMaxPushUp_ > nowMax) {
				kMaxPushUp_ = nowMax;
			}

			ImGui::Text("\n");

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			// 要素の座標操作
			indexCount_ = 0;
			for (PushUp* object : pushUps_) {
				// 名前のタグ
				std::string fullTag = "PushUp" + std::to_string(indexCount_);
				ImGui::DragFloat3(fullTag.c_str(), &object->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				ImGui::Text("\n");
				indexCount_++;
			}

			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		// タブバーを終了
		ImGui::EndTabBar();
	}

	ImGui::End();

}

void GameEditor::SystemImGui()
{

	ImGui::Begin("CommonSystem");
	ImGui::SeparatorText("Object");
	ImGui::DragFloat3("GroundPos", &ground_->transform_.translate_.x, 0.01f, -10.0f, 10.0f);
	// 保存の名前
	static char saveName[256];
	ImGui::InputText("SaveName", saveName, IM_ARRAYSIZE(saveName));
	ImGui::Text(saveName_.c_str());
	int num = GameDataManager::LoadNumber;
	ImGui::Text("%d", num);
	if (ImGui::Button("NameSave")) {
		saveName_ = saveName;
		dataManager_->AddSingleAttack(saveName_);
		dataManager_->AddLoadNum();
	}

	ImGui::Text("\n");

	if (ImGui::BeginTabBar("EditObject")) {

		if (ImGui::BeginTabItem("Camera")) {
			//ImGui::SameLine();
			if (ImGui::RadioButton("UpSide", &cameraType_, kUpSide)) {
				CameraUpdate();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("GameCamera", &cameraType_, kGameSide)) {
				CameraUpdate();
			}

			ImGui::EndTabItem();
		}

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
	pushUps_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

void GameEditor::CreateCamera()
{
	// カメラ初期化
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	//camera_->transform_.translate_ = { 0.0f,47.0f,-85.0f };
	cameraType_ = kUpSide;
	// 角度・座標変更
	CameraUpdate();
	// 地面
	ground_ = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	ground_->transform_.translate_.y = -1.5f;
	ground_->transform_.scale_ = { 55.81f,1.0f,32.5f };
}

void GameEditor::SaveInfoParameter()
{
	// 名前
	HierarchicalName names = GetNormalInfo();
	// 最大数の設定
	dataManager_->SetValue(names, "MaxCount", kMaxMeteor_);
	dataManager_->SetValue(names, "Scale", size_);
	dataManager_->SetValue(names, "Distance", respawnDistance_);
	names = { "MeteorParam","Multi" };
	dataManager_->SetValue(names, "CoolTime", coolTime_);
	dataManager_->SetValue(names, "MaxCount", kMaxCount_);
}
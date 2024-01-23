#include "GameEditor.h"
#include "../../../Base/WinApp.h"
#include "../../../Math/Math.h"

void GameEditor::Init()
{
	dataManager_ = GameDataManager::GetInstance();
	gameObjectManager_ = GameObjectManager::GetInstance();

	CreateCamera();

	ParameterInitialize();

	dataManager_->CreateGroup({ "Test","A" });
	std::list<std::string> nameList;
	nameList.push_back("Multi");
	nameList.push_back("Single");
	nameList.push_back("Roller");
	nameList.push_back("PushUp");
	dataManager_->SetValue({ "Test","List" }, "charList", nameList);
	dataManager_->SaveData("Test");

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
	HierarchicalName names;
	std::string MaxCountName = "MaxCount";

#pragma region 共通のパラメータ
	// 名前
	names = GetNormalInfo();
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "IsGravity", isGravity_);
	dataManager_->AddItem(names, "Scale", size_);
	dataManager_->AddItem(names, "Distance", respawnDistance_);

	// Jsonから受け取る
	meteorCounter_ = dataManager_->GetValue<int>(names, MaxCountName);
	isGravity_ = dataManager_->GetValue<int>(names, "IsGravity");
	size_ = dataManager_->GetValue<Vector3>(names, "Scale");
	respawnDistance_ = dataManager_->GetValue<float>(names, "Distance");

	// 名前
	names.kSection = "Multi";
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "CoolTime", coolTime_);
	dataManager_->AddItem(names, MaxCountName, kMaxCount_);

	// Jsonから受け取る
	coolTime_ = dataManager_->GetValue<float>(names, "CoolTime");
	kMaxCount_ = dataManager_->GetValue<int>(names, MaxCountName);

	names.kSection = "PushUp";
	// パラメータない用
	dataManager_->AddItem(names, MaxCountName, pushUpCounter_);

	pushUpCounter_ = dataManager_->GetValue<int>(names, MaxCountName);

	names.kSection = "Roller";
	// 
	dataManager_->AddItem(names, MaxCountName, rollerCounter_);

	rollerCounter_ = dataManager_->GetValue<int>(names, MaxCountName);

#pragma endregion

#pragma region 通常落下部分
	// 名前
	names = GetSingleInfo();
	// 隕石ごとに
	for (int i = 0; i < meteorCounter_; i++) {
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

	for (int i = 0; i < pushUpCounter_; i++) {
		PushUp* object;
		object = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);

		// パス生成
		std::string group = kPushAttackName; // ここを＋添え字で複数管理できるようにする
		// 一つひとつのオブジェクト
		std::string section = "PushUp" + std::to_string(i);
		// 値
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

#pragma region ローラーの攻撃

	for (int i = 0; i < rollerCounter_; i++) {
		Roller* object;
		object = gameObjectManager_->CreateInstance<Roller>("Roller", BaseObject::TagMeteor);

		// パス生成
		std::string group = kRollerAttackName;
		// 
		std::string section = "Roller" + std::to_string(i);
		// 値
		std::string key = "Position";

		Vector3 newPos = {};
		dataManager_->AddItem({ group,section }, key, newPos);
		newPos = dataManager_->GetValue<Vector3>({ group,section }, key);

		object->transform_.translate_ = newPos;
		object->SetVelocity({});
		// 
		rollers_.push_back(object);
	}

#pragma endregion


}

void GameEditor::EditorImGui()
{
	counter_ = 0;

	ImGui::Begin("Editor");
	ImGui::SeparatorText("DataController");
	// ボス用の保存ボタン
	if (ImGui::Button("BossSave")) {
		// 名前
		HierarchicalName names;
		// まとめてくるやつ
		std::string single = kSingleAttackName;
		// 突き上げ
		std::string pushUp = kPushAttackName;
		// 基本情報
		std::string param = kParamName;
		// ローラー
		std::string roller = kRollerAttackName;

		SaveInfoParameter();
		//---通常メテオ---//
		// 数初期化
		counter_ = 0;
		for (Meteor* object : meteors_) {
			SetObject(object, single, 0);
			ImGui::Text("\n");
			counter_++;
		}

		//---突き上げ---//
		// 数初期化
		counter_ = 0;
		for (PushUp* object : pushUps_) {
			SetObject(object, pushUp, 1);
			ImGui::Text("\n");
			counter_++;
		}

		//---ローラー---//
		// 数初期化
		counter_ = 0;
		for (Roller* object : rollers_) {
			SetObject(object, roller, 2);
			ImGui::Text("\n");
			counter_++;
		}

		//---保存処理---//

		// ローラーのファイル
		dataManager_->SaveData(kRollerAttackName);
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
		const int nowMax = 20;

#pragma region 通常のまとめて落下
		// 上から落ちてくるやつの方
		if (ImGui::BeginTabItem("SingleAttack")) {
			ImGui::SeparatorText("System");
			if (ImGui::Button("Save")) {
				// 名前
				std::string single = "SingleMeteor";
				// 数初期化
				counter_ = 0;
				for (Meteor* meteor : meteors_) {
					SetObject(meteor, single, 0);
					ImGui::Text("\n");
					counter_++;
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
			ImGui::InputInt("MeteorMaxCount", &meteorCounter_,0);

			// 敵の追加
			if (ImGui::Button("Add")) {
				// カウントアップ
				meteorCounter_++;
				// 最大を超えないように
				if (meteorCounter_ > nowMax) {
					meteorCounter_ = nowMax;
					ImGui::EndTabItem();
					ImGui::EndTabBar();
					ImGui::End();
					return;
				}
				// インスタンス生成
				Meteor* meteor;
				meteor = gameObjectManager_->CreateInstance<Meteor>("Meteor", BaseObject::TagNone);
				std::string group = "SingleMeteor";
				std::string section = "Meteor" + std::to_string(meteorCounter_);
				Vector3 newPos{ 0,2.0f,0 };
				dataManager_->AddItem({ group,section }, "Position", newPos);
				meteor->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				meteors_.push_back(meteor);
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete")) {
				//
				if (meteorCounter_ > 0) {
					meteorCounter_--;
					meteors_.pop_back();
				}
				else {
					meteorCounter_ = 0;
				}
			}

			ImGui::Text("\n");

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			for (Meteor* meteor : meteors_) {
				// 名前のタグ
				std::string fullTag = "Meteor" + std::to_string(counter_);
				// 座標
				ImGui::DragFloat3(fullTag.c_str(), &meteor->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				ImGui::Text("\n");
				counter_++;
			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
#pragma endregion

#pragma region 追尾攻撃
		// 追尾の方
		if (ImGui::BeginTabItem("MultiAttack")) {
			ImGui::SeparatorText("System");
			// 保存
			if (ImGui::Button("Save")) {
				HierarchicalName names = { "AttackParam","Multi" };
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
#pragma endregion

#pragma region 突き上げ
		// 下からのやつ
		if (ImGui::BeginTabItem("PushUpAttack")) {
			// 保存
			ImGui::SeparatorText("System");
			if (ImGui::Button("Save")) {
				// 名前
				std::string pushUp = "PushUpAttack";
				// 数初期化
				counter_ = 0;
				for (PushUp* object : pushUps_) {
					SetObject(object, pushUp, 1);
					ImGui::Text("\n");
					counter_++;
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
			ImGui::InputInt("PushMaxCount", &pushUpCounter_);

			// 敵の追加
			if (ImGui::Button("Add")) {
				// カウントアップ
				pushUpCounter_++;
				// 最大を超えないように
				if (pushUpCounter_ > nowMax) {
					pushUpCounter_ = nowMax;
					ImGui::EndTabItem();
					ImGui::EndTabBar();
					ImGui::End();
					return;
				}
				// インスタンス生成
				PushUp* object;
				object = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
				std::string group = "PushUpAttack";
				std::string section = "PushUp" + std::to_string(pushUpCounter_);
				Vector3 newPos = {};
				dataManager_->AddItem({ group,section }, "Position", newPos);
				object->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				pushUps_.push_back(object);
			}

			ImGui::Text("\n");

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			// 要素の座標操作
			counter_ = 0;
			for (PushUp* object : pushUps_) {
				// 名前のタグ
				std::string fullTag = "PushUp" + std::to_string(counter_);
				ImGui::DragFloat3(fullTag.c_str(), &object->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				ImGui::Text("\n");
				counter_++;
			}

			ImGui::EndChild();

			ImGui::EndTabItem();
		}
#pragma endregion

#pragma region ローラー
		
		if (ImGui::BeginTabItem("RollerAttack")) {

			// 保存
			ImGui::SeparatorText("System");
			if (ImGui::Button("Save")) {
				// 名前
				std::string roller = kRollerAttackName;
				// 数初期化
				counter_ = 0;
				for (Roller* object : rollers_) {
					SetObject(object, roller, 2);
					ImGui::Text("\n");
					counter_++;
				}
				// 隕石のファイル
				dataManager_->SaveData(roller);
				// セーブが完了したことをメッセージボックスで出す
				std::string message = std::format("{}.json saved.", roller);
				MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
			}

			// 要素
			ImGui::SeparatorText("Individual");
			// 最大の数
			ImGui::InputInt("RollerMaxCount", &rollerCounter_);

			// 敵の追加
			if (ImGui::Button("Add")) {
				// カウントアップ
				rollerCounter_++;
				// 最大を超えないように
				if (rollerCounter_ > nowMax) {
					rollerCounter_ = nowMax;
					ImGui::EndTabItem();
					ImGui::EndTabBar();
					ImGui::End();
					return;
				}
				// インスタンス生成
				Roller* object;
				object = gameObjectManager_->CreateInstance<Roller>("Roller", BaseObject::TagMeteor);
				std::string group = kRollerAttackName;
				std::string section = "Roller" + std::to_string(rollerCounter_);
				Vector3 newPos = {};
				dataManager_->AddItem({ group,section }, "Position", newPos);
				object->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				object->SetVelocity({});
				rollers_.push_back(object);
			}

			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			// 要素の座標操作
			counter_ = 0;
			for (Roller* object : rollers_) {
				// 名前のタグ
				std::string fullTag = "Roller" + std::to_string(counter_);
				ImGui::DragFloat3(fullTag.c_str(), &object->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				ImGui::Text("\n");
				counter_++;
			}

			ImGui::EndChild();

			ImGui::EndTabItem();
		}
#pragma endregion

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

			std::list<std::string> testList = dataManager_->GetValue<std::list<std::string>>({ "Test","List" }, "charList");

			for (std::string st : testList) {
				ImGui::Text(st.c_str());
				ImGui::Text("\n");
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Table")) {
			// 操作するアイテム一覧
			static const char* itemNames[] = { "Single","Multi","PushUp","Roller" };

			for (int i = 0; i < IM_ARRAYSIZE(itemNames); i++) {
				// アイテムそれぞれを設定
				const char* item = itemNames[i];
				ImGui::Selectable(item);

				// つかんでいる場合
				if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
					int next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
					if (next >= 0 && next < IM_ARRAYSIZE(itemNames)) {
						itemNames[i] = itemNames[next];
						itemNames[next] = item;
						ImGui::ResetMouseDragDelta();
					}

				}
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
	std::string max = "MaxCount";
	// 最大数の設定
	dataManager_->SetValue(names, max, meteorCounter_);
	dataManager_->SetValue(names, "Scale", size_);
	dataManager_->SetValue(names, "Distance", respawnDistance_);

	// ローラー
	names.kSection = "Roller";
	dataManager_->SetValue(names, max, rollerCounter_);

	// 突き上げ
	names.kSection = "PushUp";
	dataManager_->SetValue(names, max, pushUpCounter_);

	// 追尾
	names.kSection = "Multi";
	dataManager_->SetValue(names, max, kMaxCount_);
	dataManager_->SetValue(names, "CoolTime", coolTime_);

}
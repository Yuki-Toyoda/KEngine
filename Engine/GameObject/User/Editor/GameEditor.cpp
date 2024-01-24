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
	dataManager_->SetValue({ "Test","Parttern0" }, "List0", nameList);
	nameList.pop_back();
	nameList.pop_back();
	dataManager_->SetValue({ "Test","Pattern1" }, "List0", nameList);
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
	if (gameManager_ == nullptr) {
		gameManager_ = gameObjectManager_->CreateInstance<GameManager>("gameManager", BaseObject::TagNone);
	}
#pragma region 共通のパラメータ
	// 名前
	names.kSection = kParamSectionName;
	names.kGroup = dataManager_->GetSingleAttack(stageNumber[kSingle]);
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, MaxCountName, meteorCounter_);
	dataManager_->AddItem(names, "IsGravity", isGravity_);
	dataManager_->AddItem(names, "Scale", singleSize_);
	dataManager_->AddItem(names, "Distance", respawnDistance_);

	// Jsonから受け取る
	meteorCounter_ = dataManager_->GetValue<int>(names, MaxCountName);
	isGravity_ = dataManager_->GetValue<int>(names, "IsGravity");
	singleSize_ = dataManager_->GetValue<Vector3>(names, "Scale");
	respawnDistance_ = dataManager_->GetValue<float>(names, "Distance");
#pragma region 通常落下部分

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
		meteor->SetgameManager(gameManager_);
		// プッシュ
		meteors_.push_back(meteor);
	}
#pragma endregion

	//---追尾---//
	names.kGroup = dataManager_->GetMultiAttack(stageNumber[kMulti]);
	// パラメータがない場合用の処理
	dataManager_->AddItem(names, "CoolTime", coolTime_);
	dataManager_->AddItem(names, MaxCountName, kMaxCount_);

	// Jsonから受け取る
	coolTime_ = dataManager_->GetValue<float>(names, "CoolTime");
	kMaxCount_ = dataManager_->GetValue<int>(names, MaxCountName);

	//---突き上げ---//
	names.kGroup = dataManager_->GetPushUpAttack(stageNumber[kPushUp]);;
	// パラメータない用
	dataManager_->AddItem(names, MaxCountName, pushUpCounter_);

	pushUpCounter_ = dataManager_->GetValue<int>(names, MaxCountName);
#pragma region 下からの攻撃

	for (int i = 0; i < pushUpCounter_; i++) {
		PushUp* object;
		object = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);

		// パス生成
		std::string group = names.kGroup;
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

	//---ローラー---//
	names.kGroup = dataManager_->GetRollerAttack(stageNumber[kRoller]);;
	// パラメータない用
	dataManager_->AddItem(names, MaxCountName, rollerCounter_);

	rollerCounter_ = dataManager_->GetValue<int>(names, MaxCountName);
#pragma region ローラーの攻撃
	for (int i = 0; i < rollerCounter_; i++) {
		Roller* object;
		object = gameObjectManager_->CreateInstance<Roller>("Roller", BaseObject::TagMeteor);

		// パス生成
		std::string group = names.kGroup;
		// 
		std::string section = "Roller" + std::to_string(i);
		// 値
		std::string key = "Position";

		Vector3 newPos = {};
		dataManager_->AddItem({ group,section }, key, newPos);
		newPos = dataManager_->GetValue<Vector3>({ group,section }, key);

		object->transform_.translate_ = newPos;
		object->SetVelocity({});
		object->SetgameManager(gameManager_);
		// 
		rollers_.push_back(object);
	}

#pragma endregion

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

		SaveAll();

		// セーブが完了したことをメッセージボックスで出す
		std::string message = std::format("{} saved.", "Data");
		MessageBoxA(nullptr, message.c_str(), "DataManager", 0);
	}
	// 更新
	if (ImGui::Button("Reload")) {
		DataReaload();
	}
	ImGui::Text("\n");

	ImGui::SeparatorText("Object");

	if (ImGui::BeginTabBar("EditObject")) {
		const int nowMax = 20;

#pragma region 通常のまとめて落下
		// 上から落ちてくるやつの方
		if (ImGui::BeginTabItem("SingleAttack")) {
			// パターンの切り替え
			ImGui::SeparatorText("Pattern");
			if (ImGui::Button("Next")) {
				if (stageNumber[kSingle] < 10) {
					SaveSingle();
					stageNumber[kSingle]++;
					DataReaload();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Prev")) {
				if (stageNumber[kSingle] > 0) {
					SaveSingle();
					stageNumber[kSingle]--;
					DataReaload();
				}
			}

			if (ImGui::Button("Save")) {
				SaveSingle();
			}

			int changeNum = stageNumber[kSingle];
			ImGui::InputInt("num", &changeNum, 0);
			if (changeNum != stageNumber[kSingle] && changeNum < 10) {
				SaveSingle();
				stageNumber[kSingle] = changeNum;
				DataReaload();
			}

			ImGui::SeparatorText("Common");
			// 落下
			static bool gravity = static_cast<bool>(isGravity_);
			if (ImGui::Checkbox("Gravity", &gravity)) {
				if (!gravity && isGravity_) {
					DataReaload();
				}

				isGravity_ = static_cast<int>(gravity);
				// パラメータのファイル保存
				SaveSingle();
			}
			// サイズ
			ImGui::DragFloat3("Meteor_Scale", &singleSize_.x, 0.01f, 1.0f, 20.0f);
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
				std::string group = dataManager_->GetSingleAttack(stageNumber[kSingle]);
				std::string section = "Meteor" + std::to_string(meteorCounter_);
				Vector3 newPos{ 0,2.0f,0 };
				dataManager_->AddItem({ group,section }, "Position", newPos);
				meteor->transform_.translate_ = dataManager_->GetValue<Vector3>({ group,section }, "Position");
				meteor->SetgameManager(gameManager_);
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
			// パターンの切り替え
			ImGui::SeparatorText("Pattern");
			if (ImGui::Button("Next")) {
				if (stageNumber[kPushUp] < 10) {
					SavePushUp();
					stageNumber[kPushUp]++;
					DataReaload();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Prev")) {
				if (stageNumber[kPushUp] > 0) {
					SavePushUp();
					stageNumber[kPushUp]--;
					DataReaload();
				}
			}

			if (ImGui::Button("Save")) {
				SavePushUp();
			}

			int changeNum = stageNumber[kPushUp];
			ImGui::InputInt("num", &changeNum, 0);
			if (changeNum != stageNumber[kPushUp] && changeNum < 10) {
				SavePushUp();
				stageNumber[kPushUp] = changeNum;
				DataReaload();
			}
			// 要素
			ImGui::SeparatorText("Individual");
			// 最大の数
			ImGui::InputInt("PushMaxCount", &pushUpCounter_,0);

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
			// 行結合
			ImGui::SameLine();
			// 削除
			if (ImGui::Button("Delete")) {
				//
				if (pushUpCounter_ > 0) {
					pushUpCounter_--;
					pushUps_.pop_back();
				}
				else {
					pushUpCounter_ = 0;
				}
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
			// パターンの切り替え
			ImGui::SeparatorText("Pattern");
			if (ImGui::Button("Next")) {
				if (stageNumber[kRoller] < 10) {
					SaveRoller();
					stageNumber[kRoller]++;
					DataReaload();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Prev")) {
				if (stageNumber[kRoller] > 0) {
					SaveRoller();
					stageNumber[kRoller]--;
					DataReaload();
				}
			}

			if (ImGui::Button("Save")) {
				SaveRoller();
			}

			int changeNum = stageNumber[kRoller];
			ImGui::InputInt("num", &changeNum, 0);
			if (changeNum != stageNumber[kRoller] && changeNum < 10) {
				SaveRoller();
				stageNumber[kRoller] = changeNum;
				DataReaload();
			}
			// 要素
			ImGui::SeparatorText("Individual");
			// 最大の数
			ImGui::InputInt("RollerMaxCount", &rollerCounter_, 0);

			// 追加
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
				object->SetgameManager(gameManager_);
				rollers_.push_back(object);
			}
			ImGui::SameLine();
			// 削除
			if (ImGui::Button("Delete")) {
				if (rollerCounter_ > 0) {
					rollerCounter_--;
					rollers_.pop_back();
				}
				else {
					rollerCounter_ = 0;
				}

			}
			// タブの内容
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 280), ImGuiWindowFlags_NoTitleBar);

			// 要素の座標操作
			counter_ = 0;
			for (Roller* object : rollers_) {
				// 名前のタグ
				std::string fullTag = "Roller" + std::to_string(counter_);
				ImGui::DragFloat3(fullTag.c_str(), &object->transform_.translate_.x, 0.1f, -kAbsValue, kAbsValue);
				Vector3 direct = object->GetVelocity();
				fullTag = "Direct" + fullTag;
				ImGui::DragFloat3(fullTag.c_str(), &direct.x, 0.01f, -1.0f, 1.0f);
				object->SetVelocity(direct);
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

	for (int i = 0; i < 10; i++) {
		std::string group = dataManager_->GetPushUpAttack(i);
		dataManager_->CreateGroup({ group,"A" });
		dataManager_->SaveData(group);
		group = dataManager_->GetRollerAttack(i);
		dataManager_->CreateGroup({ group,"A" });
		dataManager_->SaveData(group);
		group = dataManager_->GetPushUpAttack(i);
		dataManager_->CreateGroup({ group,"A" });
		dataManager_->SaveData(group);
	}

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

			std::list<std::string> testList = dataManager_->GetValue<std::list<std::string>>({ "Test","Parttern0" }, "List0");

			for (std::string st : testList) {
				ImGui::Text(st.c_str());
				ImGui::Text("\n");
			}

			std::list<std::string> list = dataManager_->GetValue<std::list<std::string>>({ "Test","Pattern1" }, "List0");
			for (std::string st : list) {
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
	ground_ = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagNone);
	ground_->transform_.translate_.y = -2.5f;
	ground_->transform_.scale_ = { 55.0f,1.0f,55.0f };
}

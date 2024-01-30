#pragma once
#include "GameDataManager.h"
#include "../../GameObjectManager.h"
#include "../../ObjectList.h"
#include "../Enemy/IEnemy.h"
#include "../../Core/Camera.h"
#include <vector>

#ifdef _DEBUG
#include "../../../../externals/imgui/imgui.h"
#endif // _DEBUG

class GameEditor
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ
	HierarchicalName GetSingleInfo() { return { "SingleMeteor","Info" }; }

private:
	// データ用マネージャ
	GameDataManager* dataManager_ = nullptr;
	// オブジェクト用マネージャ
	GameObjectManager* gameObjectManager_ = nullptr;
	// ゲームシーンで使用するカメラ
	InGameCamera* camera_;
	// 地面
	Ground* ground_;

	/// <summary>
	/// エディターのImGui
	/// </summary>
	void EditorImGui();
	/// <summary>
	/// システム的な部分のImGui
	/// </summary>
	void SystemImGui();

	/// <summary>
	/// カメラの位置角度変更
	/// </summary>
	void CameraUpdate() {
		if (cameraType_ == kUpSide) {
			camera_->transform_.translate_ = { 0,150.0f,0 };
			camera_->transform_.rotate_ = { 1.57f,0.0f,0.0f };
		}
		else if (cameraType_ == kGameSide) {
			camera_->transform_.translate_ = { 0.0f,47.0f,-90.0f };
			camera_->transform_.rotate_ = { 0.55f,0.0f,0.0f };
		}
	}

public:
	/// <summary>
	/// Jsonから情報取得の関数
	/// </summary>
	void ParameterInitialize();
	/// <summary>
	/// データのリロード
	/// </summary>
	void DataReaload();
	/// <summary>
	/// カメラ作成
	/// </summary>
	void CreateCamera();
	/// <summary>
	/// 修正用関数
	/// </summary>
	void ObjectFix(){
		for (Meteor* meteor : meteors_) {
			meteor->transform_.scale_ = singleSize_;
			if (meteor->transform_.translate_.y < 2.0f) {
				meteor->transform_.translate_.y = 2.0f;
			}
		}

		for (Roller* roller : rollers_) {
			roller->transform_.scale_ = rollerSize_;
		}

		for (PushUp* object : pushUps_) {
			object->transform_.scale_ = pushUpSize_;
		}

	}

private:
	/// <summary>
	/// 保存関数
	/// </summary>
	void SaveAll();
	/// <summary>
	/// オブジェクト保存関数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="list"></param>
	/// <param name="groupName"></param>
	/// <param name="index"></param>
	template<typename T>
	void SaveObject(std::vector<T*> list, std::string groupName, int index) {
		std::string group = groupName/* + std::to_string(stageNumber[index])*/;

		counter_ = 0;
		for (T* object : list) {
			SetObject(object, group, index);
			ImGui::Text("\n");
			counter_++;
		}
		dataManager_->SaveData(group);
	}
	/// <summary>
	/// オブジェクト保存
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="target"></param>
	/// <param name="index"></param>
	template<typename T>
	void SetObject(T* target, const std::string& groupName, int index) {
		// 名前
		std::string groupPath = groupName;
		// セクションパス
		std::string sectionPath = dataManager_->kObjectNames[index] + std::to_string(counter_);
		// アイテムパス
		std::string keyPath = "Position";
		// 座標設定
		Vector3 setPos = {};
		dataManager_->AddItem({ groupPath,sectionPath }, keyPath, target->transform_.translate_);
		setPos = target->transform_.translate_;
		setPos.y = 0;
		dataManager_->SetValue({ groupPath,sectionPath }, keyPath, target->transform_.translate_);
	}
	/// <summary>
	/// 上からのやつの保存
	/// </summary>
	void SaveSingle();
	/// <summary>
	/// 下からのやつの保存
	/// </summary>
	void SavePushUp();
	/// <summary>
	/// ローラの保存
	/// </summary>
	void SaveRoller();
	/// <summary>
	/// 追尾の保存
	/// </summary>
	void SaveMulti();

	/// <summary>
	/// リストの変換(一文字専用
	/// </summary>
	/// <param name="lists"></param>
	/// <returns></returns>
	std::list<int> ConvertToIntList(std::list<std::string> lists) {

		std::list<int> list;
		int returnNum = 0;

		for (std::string obj : lists) {

			for (char c : obj) {
				if (std::isdigit(c)) {
					returnNum = std::stoi(obj);
					list.push_back(returnNum);
				}
			}

		}
		return list;
	}

	std::list<std::string> ConvertToStringList(std::list<int> lists) {
		std::list<std::string> stringList;
		for (int number : lists) {
			std::string sNumber = std::to_string(number);
			stringList.push_back(sNumber);
		}
		return stringList;
	}

	void SaveTableData(int number) {
		std::string groupName = "TableData";
		std::string sectionName = "TableList" + std::to_string(number);

		dataManager_->SetValue({ groupName,sectionName }, "ActionList", tableNames_);
		dataManager_->SetValue({ groupName,sectionName }, "NumberList", tableNumbers_);

		dataManager_->SaveData(groupName);
	}

	void ReleadTable();

private:
	// 保存の名前
	std::string saveName_ = "";

	enum CameraType {
		kUpSide,
		kGameSide,
	};

	std::list<std::string> tableNames_;

	std::list<std::string> numberStrings_;
	std::list<int> tableNumbers_;

	int cameraType_ = kUpSide;

	int type_ = 0;
	int tmpInt = 0;

	//int tableNumber_ = 0;

	int nowTableNumber_ = 0;

	// 攻撃の種類ごとの名前（グループネームのベース)
	const std::string kParamName = "AttackParam";
	const std::string kSingleAttackName = "SingleMeteor";
	const std::string kMultiAttackName = "MultiMeteor";
	const std::string kPushAttackName = "PushUpAttack";
	const std::string kRollerAttackName = "RollerAttack";
	const std::string kParamSectionName = "Parameter";

	float kAbsValue = 50.0f;

	enum AttackType {
		kSingle,
		kPushUp,
		kRoller,
		kMulti,
	};

	std::array<int, 4> stageNumber = {};

private:
	/// SingleAttack用
	// 隕石の数
	int meteorCounter_ = 0;
	// 敵の最大数
	int enemyCounter_ = 0;
	// 上がってくるオブジェクトの数
	int pushUpCounter_ = 0;
	// ローラの数
	int rollerCounter_ = 0;
	// for文の際の値
	int counter_ = 0;
	// 隕石の大きさ
	Vector3 singleSize_ = { 1,1,1 };
	Vector3 multiSize_ = { 1,1,1 };
	Vector3 pushUpSize_ = { 1,1,1 };
	Vector3 rollerSize_ = { 8.0f,1,1.0f };
	// 落下かどうか
	int isGravity_ = 0;
	// スポーンY座標用
	float respawnDistance_ = 30.0f;

	/// MultiAttack用
	//攻撃のクールタイム
	float coolTime_ = 1.0f;
	//何回攻撃するか
	int kMaxCount_ = 5;

private: // 仮配置用のオブジェクト
	// 隕石
	std::vector<Meteor*> meteors_;

	std::vector<PushUp*> pushUps_;

	std::vector<Roller*> rollers_;

	GameManager* gameManager_ = nullptr;

};


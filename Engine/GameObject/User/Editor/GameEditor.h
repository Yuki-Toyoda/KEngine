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
	HierarchicalName GetNormalInfo() { return { "MeteorParam","Info" }; }
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
	// プレイヤー
	Player* player_;

	static int sLoadNumber_;
	/// <summary>
	/// エディターのImGui
	/// </summary>
	void EditorImGui();

	void SystemImGui();

	/// <summary>
	/// カメラの位置角度変更
	/// </summary>
	void CameraUpdate() {
		if (cameraType_ == kUpSide) {
			camera_->transform_.translate_ = { 0,100.0f,0 };
			camera_->transform_.rotate_ = { 1.57f,0.0f,0.0f };
		}
		else if (cameraType_ == kGameSide) {
			camera_->transform_.translate_ = { 0.0f,47.0f,-85.0f };
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
	/// 隕石の座標修正用
	/// </summary>
	void MeteorFix(){
		for (Meteor* meteor : meteors_) {
			meteor->transform_.scale_ = size_;
			if (meteor->transform_.translate_.y < 2.0f) {
				meteor->transform_.translate_.y = 2.0f;
			}
		}
	}

private:
	/// <summary>
	/// オブジェクト保存
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="target"></param>
	/// <param name="index"></param>
	template<typename T>
	void SetObject(T* target,const std::string& groupName, int index) {
		// 名前
		std::string groupPath = groupName;
		// セクションパス
		std::string sectionPath = dataManager_->kObjectNames[index] + std::to_string(indexCount_);
		// アイテムパス
		std::string keyPath = dataManager_->kEnemyItems[0];
		// 座標設定
		dataManager_->AddItem({ groupPath,sectionPath }, keyPath, target->transform_.GetWorldPos());
		Vector3 setPos = target->transform_.GetWorldPos();
		setPos.y = 0;
		dataManager_->SetValue({ groupPath,sectionPath }, keyPath, target->transform_.GetWorldPos());
	}

	/// <summary>
	/// 保存関数
	/// </summary>
	void SaveInfoParameter();

private:
	// 保存の名前
	std::string saveName_ = "";

	enum CameraType {
		kUpSide,
		kGameSide,
	};

	int cameraType_ = kUpSide;


	// 攻撃の種類ごとの名前（グループネームのベース)
	const std::string kParamName = "MeteorParam";
	const std::string kSingleAttackName = "SingleMeteor";
	const std::string kMultiAttackName = "MultiMeteor";
	const std::string kPushAttackName = "PushUpAttack";
	const std::string kRollerAttackName = "RollerAttack";

	float kAbsValue = 50.0f;

private:
	/// SingleAttack用
	// 障害物の最大数
	int kMaxObstacleCount_ = 0;
	// 隕石の数
	int kMaxMeteor_ = 0;
	// 敵の最大数
	int kMaxEnemyCount_ = 0;
	// 上がってくるオブジェクトの数
	int kMaxPushUp_ = 0;
	// for文の際の値
	int indexCount_ = 0;
	// 隕石の大きさ
	Vector3 size_ = { 1,1,1 };
	// 現在の難易度
	int editNowLevel_ = 0;
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

};


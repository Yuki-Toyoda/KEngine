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

	Player* player_;

public:
	/// <summary>
	/// Jsonから情報取得の関数
	/// </summary>
	void ParameterInitialize();
	/// <summary>
	/// エディターのImGui
	/// </summary>
	void ImGuiUpdate();
	/// <summary>
	/// データのリロード
	/// </summary>
	void DataReaload();

	void CreateCamera();

private:

	template<typename T>
	void SaveObject(T* target, int index) {
		// 名前
		std::string groupPath = "SingleMeteor";
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

	//void InitObstacle(int index) {
	//	HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[0] };
	//	Obstacle* obstacle;
	//	// オブジェクトマネージャーに追加
	//	obstacle = gameObjectManager_->CreateInstance<Obstacle>("Obstacle", BaseObject::TagObstacle);
	//	// パスの設定
	//	std::string sectionPath = dataManager_->kObjectNames[1] + std::to_string(index);
	//	std::string keyPath = dataManager_->kObstacleItems[0];
	//	// 座標設定
	//	Vector3 newPos = {};
	//	dataManager_->AddItem({ names.kGroup,sectionPath }, keyPath, newPos);
	//	newPos = dataManager_->GetVector3Value({ names.kGroup,sectionPath }, keyPath);
	//	obstacle->transform_.translate_ = newPos;
	//	// リスト追加
	//	obstacles_.push_back(obstacle);
	//}

private:
	void SaveInfoParameter() {
		// 名前
		HierarchicalName names = GetNormalInfo();
		// 最大数の設定
		dataManager_->SetValue(names, "MaxCount", kMaxMeteor_);
		dataManager_->SetValue(names, "Scale", size_);
		names = { "MeteorParam","Multi" };
		dataManager_->SetValue(names, "CoolTime", coolTime_);
		dataManager_->SetValue(names, "MaxCount", kMaxCount_);
	}

private:
	// 障害物の最大数
	int kMaxObstacleCount_ = 0;
	// 隕石の数
	int kMaxMeteor_ = 0;
	// 敵の最大数
	int kMaxEnemyCount_ = 0;
	// for文の際の値
	int indexCount_ = 0;
	// 隕石の大きさ
	Vector3 size_ = { 1,1,1 };
	// 現在の難易度
	int editNowLevel_ = 0;
	// 落下かどうか
	int isGravity_ = 0;

	//攻撃のクールタイム
	float coolTime_ = 1.0f;
	//何回攻撃するか
	int kMaxCount_ = 5;

private: // 仮配置用のオブジェクト
	// 隕石
	std::vector<Meteor*> meteors_;
};


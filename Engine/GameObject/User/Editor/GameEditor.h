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

private:
	// データ用マネージャ
	GameDataManager* dataManager_ = nullptr;
	// オブジェクト用マネージャ
	GameObjectManager* gameObjectManager_ = nullptr;
	// ゲームシーンで使用するカメラ
	InGameCamera* camera_;

	Ground* ground_;

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
		std::string groupPath = "Boss";
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

	template<typename T>
	void LoadObject(T* target, int index) {
		target, index;
	}

	void InitObstacle(int index) {
		HierarchicalName names = { dataManager_->kLevelNames[editNowLevel_], dataManager_->kObjectNames[0] };
		Obstacle* obstacle;
		// オブジェクトマネージャーに追加
		obstacle = gameObjectManager_->CreateInstance<Obstacle>("Obstacle", BaseObject::TagObstacle);
		// パスの設定
		std::string sectionPath = dataManager_->kObjectNames[1] + std::to_string(index);
		std::string keyPath = dataManager_->kObstacleItems[0];
		// 座標設定
		Vector3 newPos = {};
		dataManager_->AddItem({ names.kGroup,sectionPath }, keyPath, newPos);
		newPos = dataManager_->GetVector3Value({ names.kGroup,sectionPath }, keyPath);
		obstacle->transform_.translate_ = newPos;
		// リスト追加
		obstacles_.push_back(obstacle);

	}

private:
	// マウス座標用（使ってない）
	Vector3 mouseV3Position_ = {};
	Vector2 mouseV2Position_ = {};

	// 障害物の最大数
	int kMaxObstacleCount_ = 0;
	// 隕石の数
	int kMaxMeteor_ = 0;
	// 敵の最大数
	int kMaxEnemyCount_ = 0;
	// for文の際の値
	int indexCount_ = 0;

	// 現在の難易度
	int editNowLevel_ = 0;

	int isGravity_ = 0;

private: // 仮配置用のオブジェクト
	// 敵
	std::vector<IEnemy*>enemies_;
	// ブロック
	std::vector<Obstacle*> obstacles_;

	std::vector<Meteor*> meteors_;

};


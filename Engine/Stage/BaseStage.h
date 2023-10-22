#pragma once
#include <string>
#include "../externals/imgui/imgui.h"
#include "../OBJ/OBJ.h"
#include "../GameObject/WorldTransform.h"
#include "../Sprite/SpriteList.h"
#include "../Collider/Collider.h"
#include "../Collider/CollisionManager.h"
#include "../GlobalVariables/GlobalVariables.h"
#include "../GameObject/GameObjectManager.h"

class StageManager;
class Player;

/// <summary>
/// 基底シーン
/// </summary>
class BaseStage {
public: // 共通メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	void commonInitialize();

	struct ItemInfo
	{
		int index_;
		Vector3 position_;
		bool isRePop_;
		int32_t popTime_;
	};

	struct GearInfo
	{
		float clearCondition_;
	};

	struct StageInfo
	{
		std::vector<ItemInfo> itemInfo_;
		GearInfo gearInfo_;
	};


public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Initialize(const StageInfo& info) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


	StageInfo GetStageInfo()const { return stageInfo_; }
	int GetItemInfoNum() const { return static_cast<int>(stageInfo_.itemInfo_.size()); }

	bool GetIsClear()const;



public: // パブリックなメンバ変数

	GlobalVariables* globalVariables_ = nullptr;

	// 次のステージ
	// 代入値がnullptrの場合はステージ変更しない
	//BaseStage* nextStage_ = nullptr;

	// ゲームオブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;

	// 入力取得
	//Input* input_ = nullptr;

protected: // メンバ関数

	/// <summary>
	/// ステージの情報を反映する
	/// </summary>
	void ApplyStageInfo();


protected: // メンバ変数

	StageManager* stageManager_ = nullptr;

	StageInfo stageInfo_;

	Player* player_ = nullptr;

	// 歯車の回転量
	float gearCondition_;

	// 現在のアイテム使用数
	int usedItem_ = 0;

};

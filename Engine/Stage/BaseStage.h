#pragma once
#include "../Base/DirectXCommon.h"
#include "../Resource/Texture/TextureManager.h"
#include "../externals/imgui/imgui.h"
#include "../GameObject/GameObjectManager.h"
#include "../Collider/CollisionManager.h"
#include "../GameObject/ObjectList.h"
#include "../Sprite/SpriteList.h"
#include "../Input/Input.h"
#include "../GlobalVariables/GlobalVariables.h"

class StageManager;

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
		Vector3 position_;
		bool isRePop_;
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

	void ApplyStageInfo();



protected: // メンバ変数

	StageManager* stageManager_ = nullptr;

	StageInfo stageInfo_;

	//Player* player_ = nullptr;

	// 歯車の回転量
	float gearCondition_;
};

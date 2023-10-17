#pragma once
#include "../Base/DirectXCommon.h"
#include "../Resource/Texture/TextureManager.h"
#include "../externals/imgui/imgui.h"
#include "../GameObject/GameObjectManager.h"
#include "../Collider/CollisionManager.h"
#include "../GameObject/ObjectList.h"
#include "../Sprite/SpriteList.h"
#include "../Input/Input.h"

/// <summary>
/// 基底シーン
/// </summary>
class BaseStage {
public: // 共通メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	virtual void commonInitialize() {
		// ゲームオブジェクトマネージャーのインスタンス取得
		gameObjectManager_ = GameObjectManager::GetInstance();
		// 入力取得
		input_ = Input::GetInstance();

	}

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

public: // パブリックなメンバ変数

	// 次のステージ
	// 代入値がnullptrの場合はステージ変更しない
	BaseStage* nextStage_ = nullptr;

	// ゲームオブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;

	// 入力取得
	Input* input_ = nullptr;

	struct ItemInfo
	{
		Vector2 position_;
		bool isRePop_;
	};

	struct GearInfo 
	{
		int32_t clearCondition_;
	};

	struct StageInfo 
	{
		std::vector<ItemInfo> itemInfo_;
		GearInfo gearInfo_;
	};

private: // プライベートなメンバ変数

	StageInfo stageInfo_;

};

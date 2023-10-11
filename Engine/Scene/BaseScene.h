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
class BaseScene {
public: // 共通メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	virtual void commonInitialize() {
		// ゲームオブジェクトマネージャーのインスタンス取得
		gameObjectManager_ = GameObjectManager::GetInstance();
		// 入力取得
		input_ = Input::GetInstance();

		// デバックカメラの生成
		Camera* debugCamera = new Camera();
		debugCamera->Initialize("DebugCamera", BaseObject::Camera);
		debugCamera->UseThisCamera(); // メインカメラに設定
		debugCamera->EnableDebug(true); // デバック有効
		// 生成したカメラをオブジェクトマネージャーに追加
		gameObjectManager_->AddGameObject(debugCamera);
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

	// 次のシーン
	// 代入値がnullptrの場合はシーン遷移しない
	BaseScene* nextScene_ = nullptr;

	// ゲームオブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;

	// 入力取得
	Input* input_ = nullptr;

};

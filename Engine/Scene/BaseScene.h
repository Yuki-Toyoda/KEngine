#pragma once
#include "Engine/Input/Input.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Model/Data/ModelDataManager.h"
#include "Engine/Model/ModelManager.h"
#include "Engine/SkyBox/SkyBox.h"
#include "App/GameObject/ObjectList.h"

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene
{
public: // メンバ関数

	// 仮想デストラクタ
	virtual ~BaseScene() = default;

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	void PreInitialize();

public: // 純粋仮想関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui();

private: // メンバ関数

	/// <summary>
	/// システムに必要なモデルの読み込み
	/// </summary>
	void SystemModelLoad();

public: // パブリックメンバ変数

	// スカイボックス
	std::unique_ptr<SkyBox> skyBox_;

protected: // 継承先メンバ変数

	// 入力検知クラス
	Input* input_ = nullptr;

	// オブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;

	// モデルデータマネージャー
	ModelDataManager* modelDataManager_ = nullptr;
	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

};


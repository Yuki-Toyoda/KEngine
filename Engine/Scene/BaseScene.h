#pragma once
#include "../Input/Input.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/ObjectList.h"

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
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

protected: // 継承先メンバ変数

	// 入力検知クラス
	Input* input_ = nullptr;

	// オブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

};


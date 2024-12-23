#pragma once
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Utility/Animation/Animation.h"

// クラスの前方宣言
class Enemy;

/// <summary>
/// 敵の行動基底クラス
/// </summary>
class IEnemyState
{
public: // コンストラクタ等

	// コンストラクタ
	IEnemyState() = default;
	// デストラクタ
	virtual ~IEnemyState() = default;

public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="enemy">敵の実体</param>
	/// <param name="anim">敵アニメーション</param>
	void PreInit(Enemy* enemy, MyAnimation* anim) { 
		enemy_ = enemy; 
		anim_ = anim;
	}

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
	virtual void DisplayImGui() {
		// 基底クラスでは記述なし 
	};

public: // アクセッサ等

	/// <summary>
	/// ステート名ゲッター
	/// </summary>
	/// <returns>ステート名</returns>
	const std::string GetStateName() { return stateName_; }

protected: // 継承先メンバ変数

	// 行動状態名
	std::string stateName_ = "";

	// プレイヤーの実体
	Enemy* enemy_ = nullptr;
	// アニメーション
	MyAnimation* anim_ = nullptr;

};


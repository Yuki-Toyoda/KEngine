#pragma once
#include <string>
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"

// クラスの前方宣言
class Player;

/// <summary>
/// プレイヤーの行動状態基底クラス
/// </summary>
class IState
{
public: // コンストラクタ等

	// コンストラクタ
	IState() = default;
	// デストラクタ
	virtual ~IState() = default;

public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="player">プレイヤーの実体</param>
	void PreInit(Player* player) { player_ = player; }

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
	std::string stateName_;

	// プレイヤーの実体
	Player* player_;

};


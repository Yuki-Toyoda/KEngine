#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"

// クラスの前方宣言
class FollowCamera;
class Enemy;

/// <summary>
/// ロックオンクラス
/// </summary>
class LockOn : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// ロックオン有効関数
	/// </summary>
	bool EnableLockOn();

	/// <summary>
	/// ロックオン解除関数
	/// </summary>
	void DisableLockOn();

	/// <summary>
	/// 追従カメラセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(FollowCamera* camera) { camera_ = camera; };

	/// <summary>
	/// ロックオンの有効状態のゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsLockOn() { return isLockOn_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// ロックオン可能な対象を検索する関数
	/// </summary>
	void SerchEnemy();

	/// <summary>
	/// ロックオン中の対象が範囲外に出ていないかを検証する関数
	/// </summary>
	/// <returns>ロックオンする対象が範囲外か</returns>
	bool IsOutOfRange();

public: // パブリックなメンバ変数

	// ロックオン対象
	const Enemy* target_ = nullptr;

private: // メンバ変数

	// ロックオン状態トリガー
	bool isLockOn_ = false;
	
	// カメラ
	FollowCamera* camera_ = nullptr;
	
	// 存在する敵リスト
	std::list<Enemy*> enemies_;

	// ロックオン最小距離
	float minDistance_ = 5.0f;
	// ロックオン最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * (float)(std::numbers::pi / 180.0f);

};


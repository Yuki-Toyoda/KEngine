#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"

class ThirdPersonCamera;
class SampleEnemy;

/// <summary>
/// ロックオンクラス
/// </summary>
class LockOn : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// 敵のリストセッター
	/// </summary>
	/// <param name="enemies">敵リスト</param>
	void SetEnemyList(const std::list<SampleEnemy*>& enemies);

	/// <summary>
	/// 三人称カメラセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetTPCamera(ThirdPersonCamera* camera) { camera_ = camera; }

private: // その他関数群

	/// <summary>
	/// 
	/// </summary>
	/// <param name="enemies"></param>
	void SerchEnemy();

	/// <summary>
	/// ロックオンの対象が範囲外にいるかどうかの検証関数
	/// </summary>
	/// <returns>範囲外か</returns>
	bool IsOutOfRange();

public: 

	//　ロックオン中の敵
	const SampleEnemy* target_ = nullptr;

private: // メンバ変数

	// 入力検知クラス
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// ロックオントリガー
	bool isLockOn_;

	// カメラ
	ThirdPersonCamera* camera_ = nullptr;

	// 敵リスト
	 std::list<SampleEnemy*> enemies_;

	 // ロックオンできる敵リスト
	 std::vector<const SampleEnemy*> lockableEnemies_;
	 // ロックオンする敵の配列番号
	 uint32_t LockOnIndex_ = 0;


	// ロックオン最小距離
	float minDisntance_ = 10.0f;
	// ロックオン最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * (float)(std::numbers::pi / 180.0f);

};


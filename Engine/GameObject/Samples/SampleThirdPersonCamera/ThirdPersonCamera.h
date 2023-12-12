#pragma once
#include "../../Core/Camera.h"

/// <summary>
/// 三人称の追従カメラ
/// </summary>
class ThirdPersonCamera : public Camera
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

public: // その他関数群

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset();

	/// <summary>
	/// カメラの追従対象セッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// 追従座標更新関数
	/// </summary>
	void UpdateTarget();

	/// <summary>
	/// オフセット計算関数
	/// </summary>
	/// <returns>計算されたオフセット</returns>
	Vector3 CalcOffset() const;

private:

	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	// 追従対象の目標角度
	float targetAngle_;
	// カメラの追従遅延量
	float trackingDelay_;

};


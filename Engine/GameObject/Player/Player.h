#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

/// <summary>
/// プレイヤー
/// </summary>
class MyPlayer : public BaseObject
{
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// カメラ回転角セッター
	/// </summary>
	/// <param name="rotation">カメラ回転角</param>
	void SetCameraRotation(const Vector3* rotation) { cameraRotation_ = rotation; }

private: // メンバ変数

	// 入力状態検知
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// カメラ回転角
	const Vector3* cameraRotation_ = nullptr;

	// 移動時の目標角度
	float targetAngle_;

};


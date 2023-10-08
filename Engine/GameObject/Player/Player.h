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

	/// <summary>
	/// 浮遊ギミック初期化関数
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新関数
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕振りギミック初期化
	/// </summary>
	void InitializeArmSwingGimmick();

	/// <summary>
	/// 腕振りギミック更新処理
	/// </summary>
	void UpdateArmSwingGimmick();

private: // メンバ変数

	// 入力状態検知
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// ワールド座標
	WorldTransform bodyTransform_; // 体
	WorldTransform headTransform_; // 頭
	WorldTransform armTransform_L_; // 左腕
	WorldTransform armTransform_R_; // 右腕

	// カメラ回転角
	const Vector3* cameraRotation_ = nullptr;

	// 移動時の目標角度
	float targetAngle_;

	// 浮遊移動サイクル
	int32_t floatingCycle_;
	// 浮遊の振幅
	float floatingAmpritude_;
	// 浮遊ギミック用媒介変数
	float floatingParameter_;

	// 腕振りサイクル
	int32_t armSwingCycle_;
	// 腕振りギミック用変数
	float armSwingParameter_;

};


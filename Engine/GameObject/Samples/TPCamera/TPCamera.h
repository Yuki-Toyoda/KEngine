#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"
#include "../../../../Externals/imgui/imgui.h"

/// <summary>
/// 三人称カメラ
/// </summary>
class TPCamera : public BaseObject {
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // アクセッサ等

	/// <summary>
	/// カメラのビュープロジェクション行列ゲッター
	/// </summary>
	/// <returns>カメラビュープロジェクション行列</returns>
	Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; }

	/// <summary>
	/// このカメラを使用するようにする関数
	/// </summary>
	void UseThisCamera();

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
	/// カメラの追従対象セッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// 追従座標更新関数
	/// </summary>
	void UpdateTarget();

private: // メンバ変数

	// カメラ用ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

	// 入力
	Input* input_;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

};


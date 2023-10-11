#pragma once
#include "../BaseObject.h"

class MyEnemy : public BaseObject
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
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollisionEnter(BaseObject* object)override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

private: // メンバ変数

	// 階層アニメーション用トランスフォーム
	WorldTransform TireTransform_R_; // 右タイヤ
	WorldTransform TireTransform_L_; // 左タイヤ

	// 移動速度
	Vector3 velocity_;

	// 接地判定
	bool isLanding_;
	// 最大落下速度
	float kMaxFallSpeed_;
	// 現在落下速度
	float fallSpeed_;
	// 落下加速度
	float kFallAcceleration_;

};


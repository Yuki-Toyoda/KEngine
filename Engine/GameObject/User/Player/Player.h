#pragma once
#include"../../BaseObject.h"
#include "../InputManager/InputManager.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player:public BaseObject
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

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;


	void OnCollisionEnter(Collider* collider)override;
private:
	void Damage();
private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveSpeed_ = 0.1f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;
	//吸収した数
	int absorptionCount_;
	//吸収したときのScaleの変化量
	float scaleForce_ = 0.1f;
	int hitPoint_;
};


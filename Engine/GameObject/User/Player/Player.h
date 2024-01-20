#pragma once
#include"../../BaseObject.h"
#include "../InputManager/InputManager.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Ground/Ground.h"
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

	void SetGround(Ground* ground) { ground_ = ground; }
private:
	//ダメージをくらう処理
	void Damage();
	//移動じの減速
	void SubtractVelocity();
private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveSpeed_ = 0.01f;
	//減速率
	float decelerationRate = 0.6f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;
	//吸収した数
	int absorptionCount_;
	//吸収したときのScaleの変化量
	float scaleForce_ = 0.1f;
	int hitPoint_;
	//ダメージをくらう時のクールタイム
	KLib::DeltaTimer hitCollTimer_;
	float hitCoolTime_=2.0f;
	Ground* ground_;
	//一フレーム前のポジション
	Vector3 prevPos_;
};


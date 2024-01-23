#pragma once
#include"../../BaseObject.h"
#include "../InputManager/InputManager.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Ground/Ground.h"
#include "State/PlayerStateList.h"
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
	void SetVelocity(Vector3 velo) { velocity_ = velo; }
	//減速率
	float decelerationRate = 0.6f;

	/// <summary>
	/// 引数で指定した行動状態に変更する関数
	/// </summary>
	/// <param name="newState">新しい行動状態</param>
	void ChangeState(std::unique_ptr<IPlayerState> newState);

	//攻撃
	void Atack();
	int GetAbsorptionCount() { return absorptionCount_; }
	void ResetAbsorptionCount() { absorptionCount_ = 0; }
	bool GetIsAtack() { return isAtack_; }
	bool SetIsAtack(bool Flag) { return isAtack_ = Flag; }
	float GetAtackPower() { return atackPower_; }

	Vector3 GetPushUpPos() { return pushUpPos_; }
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
	//攻撃力
	float atackForce_ = 1.0f;
	float atackPower_;
	//攻撃しているかどうか
	bool isAtack_;
	// 行動状態クラス
	std::unique_ptr<IPlayerState> state_;
	//当たったPushUpのポジション
	Vector3 pushUpPos_;
};


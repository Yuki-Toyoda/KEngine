#pragma once
#include "../../../baseObject.h"
#include "../../../../Utility/Timer/DeltaTimer.h"
#include "../../../../Utility/Lerp/lerp.h"

/// <summary>
/// 下からの突き上げ攻撃
/// </summary>
class PushUp :public BaseObject
{
private: // サブクラス

	/// <summary>
	/// PushUp用段階行動列挙子
	/// </summary>
	enum PushUpStep {
		kNone,		// 何も行わない
		kPreAttack, // 攻撃前
		kAttacking, // 攻撃中
		kPostAttack, // 攻撃後
		kAttackEnd // 攻撃終了
	};

public:
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

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset();

	/// <summary>
	/// 攻撃前処理関数
	/// </summary>
	void PreAttack();
	
	/// <summary>
	/// 攻撃中処理関数
	/// </summary>
	void Atacking();
	
	/// <summary>
	/// 攻撃後処理関数
	/// </summary>
	void PostAttack();

	/// <summary>
	/// 攻撃終了関数
	/// </summary>
	void AtackEnd();

	/// <summary>
	/// (ユーザー呼び出し禁止)処理を開始させる関数
	/// </summary>
	void SetActive();

private: // メンバ変数

	// モデル自体のトランスフォーム
	WorldTransform modelTransform_;

	// モデル色
	Vector4 modelColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 攻撃段階格納用
	int step_ = kNone;

	// 移動ベクトル
	Vector3 velocity_;

	// 攻撃トリガー
	bool isAttack_ = false;

	//タイマー
	KLib::DeltaTimer timer_;
	//攻撃前の回転にかかる時間
	float preAttackTime_= 1.0f;
	//攻撃の時間
	float attackTime_ = 0.5f;
	//判定の残る時間
	float remainTime_ = 4.0f;
	//下がったときの座標
	float rawPosition_;
	//上がったときの座標
	float upPosition_;
	//通常の高さ
	float defaultPosition_;
};


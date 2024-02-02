#pragma once
#include "../../../BaseObject.h"
#include "../../../../Audio/Audio.h"
#include "../../../../Utility/Animation/AnimationManager.h"

/// <summary>
/// ウリ丸
/// </summary>
class Uribo :public BaseObject
{
public:
	void Init()override;
	void Update()override;
	void DisplayImGui()override;
	void OnCollision(Collider* collider)override;
	/// <summary>
/// 回復関数
/// </summary>
	void Heal(int healPower);
public://アクセッサ
/// <summary>
/// 体力のげった
	int GetHP() { return hitPoint_; }
/// </summary>

	/// <summary>
	/// HP最大値のゲッター
	/// </summary>
	/// <returns>HP最大値</returns>
	int GetDefaultHP() { return defaultHP_; }

	/// <summary>
	/// ボスの死亡状態トリガーゲッター
	/// </summary>
	/// <param name="isBossDead">true or false</param>
	void SetIsBossDead(const bool& isBossDead) { isBossDead_ = isBossDead; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 調整項目クラス
	/// </summary>
	void SetGlobalVariables();

public: // パブリックなメンバ変数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;

private: // メンバ変数

	// 餌を与えられるエリアのトランスフォーム
	WorldTransform feedAreaTransform_;

	// エリア色
	Vector4 areaColor_ = { 1.0f, 1.0f, 1.0f, 0.75f };

	// 餌を与えられるエリアのメッシュ
	Mesh* feedAreaMesh_ = nullptr;

	//体力
	int hitPoint_;
	//開始時の体力
	int defaultHP_= 6000;
	//毎フレーム減らす体力
	int decrementHP = 1;
	Vector3 collisionScale_;

	// ボスの死亡トリガー
	bool isBossDead_ = false;

	// 餌を与えられる状態トリガー
	bool canFeed_ = false;

};


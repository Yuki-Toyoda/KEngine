#pragma once
#include "../../../BaseObject.h"
#include "../../../../Audio/Audio.h"

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

/// <summary>
/// チュートリアル用の関数
/// </summary>
	void Tutrial();
	bool TutrialEnd();
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
	/// ボスの死亡トリガー状態セッター
	/// </summary>
	/// <param name="isBossDead">ボスの死亡トリガー状態</param>
	void SetIsBossDead(const bool& isBossDead) { isBossDead_ = isBossDead; }

private:

	void SetGlobalVariables();
private:

	// 音再生クラス
	

	// ゲージ背景トランスフォーム
	WorldTransform gageBGTransform_;
	WorldTransform gageTransform_;

	// 
	BillboardPlane* gageBGPlane_ = nullptr;
	BillboardPlane* gagePlane_ = nullptr;

	Vector4 bgColor_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector4 gageColor_ = { 1.0f, 0.1f, 0.0f, 1.0f };

	// 大きさ
	Vector2 scale_ = { 10.0f, 1.0f };
	// 中心点
	Vector2 anchorPoint_ = { 0.5f, 0.5f };

	//体力
	int hitPoint_;
	//開始時の体力
	int defaultHP_= 6000;
	//毎フレーム減らす体力
	int decrementHP = 1;
	Vector3 collisionScaele_;

	// ボスの死亡トリガー
	bool isBossDead_ = false;

};


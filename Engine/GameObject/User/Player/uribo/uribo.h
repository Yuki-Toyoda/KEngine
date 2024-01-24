#pragma once
#include "../../../BaseObject.h"
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

private:

	void SetGlobalVariables();
private:
	//体力
	int hitPoint_;
	//開始時の体力
	int defaultHP_=10000;
	//毎フレーム減らす体力
	int decrementHP = 1;
	Vector3 collisionScaele_;
};


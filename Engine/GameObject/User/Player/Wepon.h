#pragma once
#include "../../BaseObject.h"
#include "../InputManager/InputManager.h"
class Wepon:public BaseObject
{
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

public:
	//追従対象
	void SetTarget(const WorldTransform* target) { target_ = target; }
	float GetDistance() { return distance_; }
	void AddParentCount() { parentCount_++; }
	void DeleteChain() { isChain_ = false; }
private:
	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;
	//追従対象からの距離
	float distance_;
	//追従対象からの角度
	float theta_;
	//回転の向き　０なら右回り1なら左回り
	bool rotateDirection_;
	Vector3 worldPos_;
	//回転しているか
	bool isMove_;
	//ペアレントしている数
	int parentCount_;
	float goalDistance_;
	float lerpCount_;
	//鎖がつながっているか
	bool isChain_;
private://メンバ関数

	void Move();
	void ChainDeleted();
	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

};


#pragma once
#include "../../BaseObject.h"
#include"../Player/Wepon.h"
#include "../../../Audio/Audio.h"
class Enemy:public BaseObject
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

	void Reset();
public:
	void SetStartTransform() { startTranslate_ = transform_.translate_; }
	//追従対象
	void SetWepon( Wepon* target) { wepon_ = target; }
	void Setveclocity(Vector3 velocity) { hitvelocity_ = velocity; }
	void SubtractVelocity();
	void SetPosition(const Vector3& position) { transform_.translate_ = position; }
private:
	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;
	

private:
	Audio* audio_;
	// 追従対象のワールド座標
 Wepon* wepon_ = nullptr;
 //初期化時の座標
 Vector3 startTranslate_;
 Vector3 worldPos_;
 bool isParent_;
 Vector3 velocity_;
 uint32_t soundHandleStick_;
 Vector3 hitvelocity_;
	
	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体
	WorldTransform wingTransform_L_; // 左羽
	WorldTransform wingTransform_R_; // 右羽

};



#pragma once
#include "../../BaseObject.h"
#include "../InputManager/InputManager.h"
#include <optional>
#include "../../../Audio/Audio.h"

/// <summary>
/// ビヘイビア
/// </summary>
enum class Behavior {
	kRoot,
	kAtack,
};

/// <summary>
/// 
/// </summary>
enum Size {
	low,
	mid,
	high,
};

/// <summary>
/// 武器クラス
/// </summary>
class Weapon:public BaseObject
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

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset();

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;
	Size GetSize() { return size_; }

	int GetParentCount() { return parentCount_; }
private:

	// 音再生用
	Audio* audio_;

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;
	//追従対象からの距離
	float distance_;
	//追従対象からの角度
	float theta_;
	//回転の向き　０なら右回り1なら左回り
	bool rotateDirection_;

	//回転しているか
	bool isMove_;
	//ペアレントしている数
	int parentCount_;
	float goalDistance_;
	float lerpCount_;
	//鎖がつながっているか
	bool isChain_;
	//攻撃待機か
	bool isAtack_;
	//	攻撃が終わったか
	bool isAtackEnd_;
	//behavior
	std::optional<Behavior>behaviorRequest_ = std::nullopt;
	Behavior behavior_ = Behavior::kRoot;
	bool isBreak_;
	float kMoveRotateForce_=0.05f;
	float moveRotateForce_ = 0.0f;
	float kAtackRotateForce_=0.2f;
	float AddMoveRotateForce_ = 0.005f;
	uint32_t soundHandleslam_;
	uint32_t soundHandleDamage_;
	Size size_;
	bool isCommbo_;
	const int kMaxCommbCount_ = 120;
	int commbCount_;

	// 当たり判定用ワールド座標
	Vector3 worldPos_;

private://メンバ関数

	void Move();
	void ChainDeleted();
	void Atack();

public:
	//追従対象
	void SetTarget(const WorldTransform* target) { target_ = target; }
	float GetDistance() { return distance_; }
	void AddParentCount() { parentCount_++; }
	void DeleteChain() { isChain_ = false; }
	Behavior GetBehavior() { return behavior_; }
	bool GetISBreak() { return isBreak_; }
	bool GetIsAtackEnd() { return isAtackEnd_; }
	void Commbo();
};

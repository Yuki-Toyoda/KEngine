#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"

// クラスの前方宣言
class TPCamera;
class Weapon;

/// <summary>
/// プレイヤー
/// </summary>
class SamplePlayer : public BaseObject
{
private: // サブクラス

	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用媒介変数
		uint32_t dashParameter_ = 0;
		// ダッシュスピード
		float dashSpeed_;
	};

public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollisionEnter(BaseObject* object)override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

	/// <summary>
	/// 追従カメラセッター
	/// </summary>
	/// <param name="tpCamera">追従カメラ</param>
	void SetTPCamera(TPCamera* tpCamera) { tpCamera_ = tpCamera; }
	/// <summary>
	/// 武器セッター
	/// </summary>
	/// <param name="weapon">武器</param>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// 通常時の行動初期化
	/// </summary>
	void BehaviorRootInitialize();
	/// <summary>
	/// 通常時の行動更新関数
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 通常時の行動初期化
	/// </summary>
	void BehaviorAttackInitialize();
	/// <summary>
	/// 攻撃時の行動更新関数
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// ダッシュ時の行動初期化関数
	/// </summary>
	void BehaviorDashInitialize();
	/// <summary>
	/// ダッシュ時の行動更新関数
	/// </summary>
	void BehaviorDashUpdate();

	/// <summary>
	/// 浮遊ギミック初期化関数
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新関数
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕振りギミック初期化
	/// </summary>
	void InitializeArmSwingGimmick();

	/// <summary>
	/// 腕振りギミック更新処理
	/// </summary>
	void UpdateArmSwingGimmick();

private: // メンバ変数

	// 入力状態検知
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	TPCamera* tpCamera_ = nullptr;
	Weapon* weapon_ = nullptr;

	// ワールド座標
	WorldTransform bodyTransform_; // 体
	WorldTransform headTransform_; // 頭
	WorldTransform armTransform_L_; // 左腕
	WorldTransform armTransform_R_; // 右腕

	// 移動時の目標角度
	float targetAngle_;

	// 接地判定
	bool isLanding_;
	// 最大落下速度
	float kMaxFallSpeed_;
	// 現在落下速度
	float fallSpeed_;
	// 落下加速度
	float kFallAcceleration_;

	// ジャンプできるか
	bool canJump_;
	// ジャンプ速度
	float jumpSpeed_;
	// 最大ジャンプ高度
	float kMaxJumpHeight_;
	// ジャンプ減衰速度
	float kJumpDecayRate_;

	// 浮遊移動サイクル
	int32_t floatingCycle_;
	// 浮遊の振幅
	float floatingAmpritude_;
	// 浮遊ギミック用媒介変数
	float floatingParameter_;

	// 腕振りサイクル
	int32_t armSwingCycle_;
	// 腕振りギミック用変数
	float armSwingParameter_;

	// ふるまい列挙子
	enum Behavior {
		kRoot, // 通常状態
		kAttack, // 攻撃状態
		kDash // ダッシュ状態
	};
	// ふるまい初期設定
	Behavior behavior_ = Behavior::kRoot;

	// ふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 攻撃状態列挙子
	enum AttackState {
		SwingOver, // 振りかぶり
		Attack, // 攻撃動作
		Wait, // 硬直
	};
	// 現在の攻撃状態
	int32_t attackState_ = SwingOver;

	// 攻撃演出用t
	float t_ = 0.0f;

	// 振りかぶり秒数
	float swingOverTime_ = 0.5f;
	// 攻撃秒数
	float attackTime_ = 0.25f;
	// 攻撃の時どれだけ前に進むか
	float attackForward_ = 10.0f;
	// 攻撃後硬直秒数
	float attackWaitTime_ = 0.5f;

	// 振りかぶり初期角度
	float swingOverStartAngle_ = (float)std::numbers::pi + (float)std::numbers::pi / 2.0f;
	// 振りかぶり終端角度
	float swingOverEndAngle_ = (float)std::numbers::pi - (float)std::numbers::pi / 3.0f;

	// 振り降ろし終端角度
	float attackEndAngle_ = (float)std::numbers::pi + (float)std::numbers::pi / 1.75f;

	// 攻撃開始地点
	Vector3 attackStartPos_ = { 0.0f, 0.0f, 0.0f };
	// 攻撃終端地点
	Vector3 attackEndPos_ = { 0.0f, 0.0f, 0.0f };

	// ダッシュ用変数
	WorkDash workDash_;

};


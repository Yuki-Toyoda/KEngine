#pragma once
#include "Engine/GameObject/IObject.h"
#include "Engine/Input/Input.h"
#include "State/StateList.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include "Engine/Audio/Audio.h"

// クラスの前方宣言
class Camera;
class FollowCamera;
class LockOn;
class Enemy;
class GameManager;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public IObject
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

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IState> newState);

	/// <summary>
	/// 行動状態の取得
	/// </summary>
	/// <returns></returns>
	const std::string GetStateName() { return state_->GetStateName(); }

	/// <summary>
	/// ダメージ処理関数
	/// </summary>
	/// <param name="translate">衝突したやつの座標</param>
	void HitDamage(const Vector3& translate);

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

public: // パブリックなメンバ変数

	// ゲームマネージャー
	GameManager* gameManager_ = nullptr;

	// 追従カメラ格納用
	FollowCamera* followCamera_ = nullptr;
	// ロックオン時のビネットエフェクト用
	float focusVignette_ = 0.0f;

	// ロックオンクラス
	LockOn* lockOn_ = nullptr;

	// 追従対象の目標角度
	float targetAngle_ = 0.0f;

	// 攻撃判定用線
	std::unique_ptr <Line> attackLine_;
	// 攻撃可能か
	bool canAttack_ = true;
	// 攻撃中か
	bool isAttacking_ = false;

	// コライダー座標
	WorldTransform colliderTransform_;

	// 頭のトランスフォーム
	WorldTransform headTransform_;

	// 移動可能か
	bool canAction_ = false;

	// HP
	int32_t hp_ = 6;
	// 死亡トリガー
	bool isDead_ = false;

	// 敵
	Enemy* enemy_ = nullptr;

	// 打ち返し時のラジアルブラー有効トリガー
	bool enableParryBlur_ = false;

	// 効果音群
	uint32_t SwingSword_ = 0u; // 素振り音
	uint32_t RotateSlash_ = 0u; // 回転切り音

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;

	// 開始前のセットアップフラグ
	bool isSetUp_ = false;

	// 武器のトランスフォーム
	WorldTransform weaponTransform_;

	// 体力UI
	Vector2 hertUITranslate_[6]; // 座標
	Vector2 hertUISize_[6];		 // サイズ

	// ヒットクールタイムタイマー
	KLib::DeltaTimer hitCoolTimeTimer_;
	// ヒットクールタイム定数値
	const float kHitCoolTime_ = 0.35f;

	// コライダーのワールド座標
	Vector3 colliderWorldPos_;
	// コライダー半径
	float colliderRadius_ = 0.25f;

	// 行動状態格納変数
	std::unique_ptr<IState> state_;
};

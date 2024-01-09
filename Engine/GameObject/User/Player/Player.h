#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"
#include "State/StateList.h"
#include "../../../Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class FollowCamera;
class LockOn;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseObject
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

private: // プライベートなメンバ関数

	/// <summary>
	/// パラメータを作成する
	/// </summary>
	/// <param name="name">作成するパラメータ</param>
	void CreateParameter(const std::string& name);

public: // パブリックなメンバ変数

	// 入力検知用
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// 追従カメラ格納用
	FollowCamera* followCamera_ = nullptr;

	// ロックオンクラス
	LockOn* lockOn_ = nullptr;

	// 追従対象の目標角度
	float targetAngle_ = 0.0f;

	// キャラクターアニメーション
	Animation* playerAnim_;

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

private: // メンバ変数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;
	// 右腕のトランスフォーム
	WorldTransform armTransform_R_;
	// 左腕のトランスフォーム
	WorldTransform armTransform_L_;

	// HP
	int32_t hp_ = 6;
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

	// アニメーションマネージャ
	AnimationManager* animManager_;

};

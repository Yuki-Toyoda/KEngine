#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class Boss;
class Camera;

/// <summary>
/// ボスのアニメーションマネージャー
/// </summary>
class BossAnimManager : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// カメラの更新関数
	/// </summary>
	void CameraUpdate();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">パラメータ名</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// アニメーション生成関数
	/// </summary>
	void CreateAnimation();

public: // アクセッサ等

	/// <summary>
	/// ボスのセッター
	/// </summary>
	/// <param name="boss">ボスの実体</param>
	void SetBoss(Boss* boss);

	/// <summary>
	/// 読み込みパラメータ変更関数
	/// </summary>
	/// <param name="name">読み込みパラメータ名</param>
	/// <param name="isChanged">実行時にそのパラメータへの強制的な遷移を行うか</param>
	void ChangeParamameter(const std::string& name, const bool& isChanged = true);

	/// <summary>
	/// アニメーションゲッター
	/// </summary>
	/// <returns>アニメーション本体</returns>
	Animation* GetAnimation() { return anim_; }

	/// <summary>
	/// 複数落下アニメーションの終了トリガー状態セッター
	/// </summary>
	/// <param name="isMultiFallEnd">複数落下アニメーションを終了させるか</param>
	void SetIsMultiFallEnd(const bool& isMultiFallEnd) { isMultiFallEnd_ = isMultiFallEnd; }

	/// <summary>
	/// 複数落下アニメーション中の攻撃アニメーション再生トリガー状態セッター
	/// </summary>
	/// <param name="isAttack">攻撃するか</param>
	void SetPlayMultiFallAttackAnim(const bool isAttack);

public: // アニメーション関数

	/// <summary>
	/// ダメージアニメーション関数
	/// </summary>
	void PlayDamageAnim();

	/// <summary>
	/// 落下攻撃アニメーション再生関数
	/// </summary>
	/// <param name="readyTime">準備時間</param>
	void PlayFallAttackAnim(float readyTime);

	/// <summary>
	/// 突き上げ攻撃アニメーション再生関数
	/// </summary>
	/// <param name="readyTime">準備時間</param>
	void PlayPushUpAttackAnim(float readyTime);

	/// <summary>
	/// 複数落下アニメーション再生関数
	/// </summary>
	void PlayMultiFallAnim();

	/// <summary>
	/// ローラー攻撃アニメーション再生関数
	/// </summary>
	/// <param name="readyTime">準備時間</param>
	void PlayRollerAttackAnim(float readyTime);

	/// <summary>
	/// 死亡アニメーション再生関数
	/// </summary>
	void PlayDeadAnim();

private: // メンバ変数

	// アニメーションマネージャー
	AnimationManager* animManager_ = nullptr;

	// アニメーション
	Animation* anim_ = nullptr;

	// ボスの実体
	Boss* boss_ = nullptr;
	
	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体
	WorldTransform armTransform_L_; // 左腕
	WorldTransform armTransform_R_; // 左腕
	WorldTransform footTransform_L_; // 左腕
	WorldTransform footTransform_R_; // 左腕

	// アニメーションタイマー
	KLib::DeltaTimer animTimer_;

	// 落下攻撃準備時間
	float fallAttackReadyTime_ = 0.5f;

	// 突き上げ攻撃待機時間
	float pushUpReadyTime_ = 0.5f;

	// 複数落下アニメーション再生中トリガー
	bool isMultiFalling_ = false;
	// 複数落下時のアニメーション終了トリガー
	bool isMultiFallEnd_ = true;
	// 複数落下アニメーション中の攻撃アニメーショントリガー
	bool playMultiFallAttackAnim_ = false;

	// ローラー攻撃待機時間
	float rollerAttackReadyTime_ = 0.5f;

	// 倒れる演出用カメラ
	Camera* stagingCamera_ = nullptr;

	// カメラオフセット
	Vector3 cameraOffset_ = { 0.0f, 0.0f, 0.0f };

};


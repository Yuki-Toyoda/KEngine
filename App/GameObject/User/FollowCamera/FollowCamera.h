#pragma once
#include "Engine/GameObject/Core/Camera.h"
#include "Engine/Utility/KLib.h"

// クラスの前方宣言
class LockOn;

/// <summary>
/// 追従カメラ
/// </summary>
class FollowCamera : public Camera
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

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// カメラの座標を動かした際の更新関数
	/// </summary>
	void UpdateTarget();

public: // アクセッサ等

	/// <summary>
	/// ロックオンクラスセッター
	/// </summary>
	/// <param name="lockOn">ロックオンクラス</param>
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
	/// <summary>
	/// ロックオンクラス
	/// </summary>
	/// <returns>ロックオン</returns>
	LockOn* GetLockOn() { return lockOn_; }

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// Z注目有効トリガーゲッター
	/// </summary>
	/// <returns>Z注目が有効か</returns>
	bool GetEnableZForcus() { return enableZForcus_; }

	/// <summary>
	/// 目標角度セッター
	/// </summary>
	/// <param name="targetAngle">目標角度</param>
	void SetTargetAngle(const float& targetAngle) { targetAngleY_ = targetAngle; }

	/// <summary>
	/// カメラの角度補正速度セッター
	/// </summary>
	/// <param name="correctionSpeed">カメラの角度補正速度</param>
	void SetCorrectionSpeed(const float& correctionSpeed) { correctionSpeed_ = correctionSpeed; }

	/// <summary>
	/// オフセットなしのビュー行列のゲッター
	/// </summary>
	/// <returns>オフセットなしのビュー行列</returns>
	const Matrix4x4 GetViewMatrixNoOffset();

	/// <summary>
	/// 衝突時のみコールバックされ続ける関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollision(Collider* collider) override;
	
	/// <summary>
	/// 衝突時にコールバックされ続ける関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionExit(Collider* collider) override;

private: // プライベートなメンバ関数

	/// <summary>
	/// オフセット計算関数
	/// </summary>
	/// <returns>計算後オフセット</returns>
	Vector3 CalcOffset() const;

private: // メンバ変数

	// ロックオンクラス
	LockOn* lockOn_ = nullptr;

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

	// オフセットなしのカメラのワールドトランスフォーム
	WorldTransform noOffsetTransform_;

	// カメラの当たり判定半径
	float colliderRadius_ = 0.1f;

	// 追従対象の目標角度
	float targetAngleX_ = 0.0f;
	float targetAngleY_ = 0.0f;
	// 床と衝突していた際のオフセット値
	float targetAngleXOffset_ = 0.0f;

	// 目標角度への補正スピード
	float correctionSpeed_ = 0.035f;

	// 通常時カメラ角度補正速度
	const float normalCorrectionSpeed_ = 0.005f;
	// Z注目時のカメラ角度補正速度
	const float zForcusCorrectionSpeed_ = 0.25f;
	// Z注目で敵をロックオンしたときのカメラ角度補正速度
	const float zEnemyForcusCorrectionSpeed_ = 0.25f;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	// カメラの追従遅延量
	const float trackingDelay_ = 0.35f;

	// Z注目有効トリガー
	bool enableZForcus_ = false;

	// ロックオン時の遷移先座標
	Vector3 lockOnTranslate_ = {};

	// ロックオン時のセットアップを完了しているか
	bool isLockOnSetUp_ = false;
	// どちらに角度を補正するか
	bool isRightLockOn_ = false;

	// オフセット
	const Vector3 kOffset_ = { 0.0f, 2.0f, -10.0f };
	Vector3 offset_ = kOffset_;

	// ロックオン時の回転角
	float kOffsetRotate_ = 0.05f;
};


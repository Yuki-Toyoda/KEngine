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
	void SetTargetAngle(const float targetAngle) { targetAngleY_ = targetAngle; }

	/// <summary>
	/// カメラの角度補正速度セッター
	/// </summary>
	/// <param name="correctionSpeed">カメラの角度補正速度</param>
	void SetCorrectionSpeed(const float correctionSpeed) { correctionSpeed_ = correctionSpeed; }

	/// <summary>
	/// オフセットなしのビュー行列のゲッター
	/// </summary>
	/// <returns>オフセットなしのビュー行列</returns>
	const Matrix4x4 GetViewMatrixNoOffset();

public: // 機能関数群

	/// <summary>
	/// パリィ時のブラー演出の開始関数
	/// </summary>
	/// <param name="stagingTime">演出時間</param>
	/// <param name="blurStrength">ブラーの強さ</param>
	/// <param name="endStagingTime">演出の終了時間</param>
	/// <param name="blurStrength">ブラー強さ</param>
	void StartParryBlur(const float stagingTime, const float endStagingTime, const float blurStrength);

private: // プライベートなメンバ関数

	/// <summary>
	/// カメラの座標を動かした際の更新関数
	/// </summary>
	void UpdateTarget();

	/// <summary>
	/// オフセット計算関数
	/// </summary>
	/// <returns>計算後オフセット</returns>
	Vector3 CalcOffset() const;

	/// <summary>
	/// Z注目更新関数
	/// </summary>
	void ZForcusUpdate();

	/// <summary>
	/// Z注目時のビネット演出の更新関数
	/// </summary>
	void ForcusVignetteUpdate();

	/// <summary>
	/// 通常時のカメラ操作関数
	/// </summary>
	void ControllUpdate();

	/// <summary>
	/// 注目時の操作関数
	/// </summary>
	void ForcusControllUpdate();

	/// <summary>
	/// パリィ時のラジアルブラー演出の更新関数
	/// </summary>
	void ParryBlurUpdate();

private: // メンバ変数

	// ロックオンクラス
	LockOn* lockOn_ = nullptr;

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

	// オフセットなしのカメラのワールドトランスフォーム
	WorldTransform noOffsetTransform_{};

	// カメラ操作可能フラグ
	bool isCanControll_ = true;
	// カメラ操作感度
	Vector2 sensitivity_ = { 0.025f, 0.025f };
	// 操作時のスティックデッドゾーン
	const float deadZone_ = 0.25f;

	// カメラ操作時の最大X軸アングル
	float maxControllAngleX_ = 0.8f;
	float minControllAngleX_ = -0.5f;
	
	float maxlerpOffsetZ_ = -16.5f;
	float minlerpOffsetZ_ = -7.5f;

	float minlerpOffsetY_ = 1.5f;

	// ロックオン時のZ軸オフセットの距離割合
	float zOffsetRatio_ = 1.75f;

	// ロックオン時のカメラ操作時の最大X軸アングル
	float maxLockOnControllAngleX_ = 0.75f;
	float minLockOnControllAngleX_ = -0.15f;
	// ロックオン時のカメラのX軸角度によって加算されるZ軸オフセット値
	float maxAddLockOnXAngleZOffset_ = -5.0f;
	float minAddLockOnXAngleZOffset_ = 0.0f;
	// ロックオン時のカメラのY軸角度によって加算されるZ軸オフセット値
	float maxAddLockOnYAngleZOffset_ = -20.0f;
	float minAddLockOnYAngleZOffset_ = 0.0f;
	// ロックオン時の敵との距離によって加算されるZ軸オフセット値
	float maxAddLockOnDistanceZOffset_ = -5.5f;
	float minAddLockOnDistanceZOffset_ = 5.0f;
	// ロックオン時のカメラ高さ
	float maxLockOnHeight_ = 1.15f;
	float minLockOnHeight_ = 0.0f;

	float directionOffset_ = 0.0f;

	// カメラの高さ最小位置
	float minPositionY_ = 0.05f;
	// カメラが地面に触れているか
	bool isTouchFloor_ = false;

	// 追従対象の目標角度
	float targetAngleX_ = 0.0f;
	float targetAngleY_ = 0.0f;

	// X軸目標角度の最大閾値
	float maxTargetAngleX_ = -0.1f;

	// 目標角度への補正スピード
	float correctionSpeed_ = 0.035f;

	// 通常時カメラ角度補正速度
	const float normalCorrectionSpeed_ = 0.005f;
	// Z注目時のカメラ角度補正速度
	const float zStartForcusCorrectionSpeed_ = 0.1f;
	// Z注目時のカメラ角度補正速度
	const float zForcusCorrectionSpeed_ = 0.25f;

	// Z注目で敵をロックオンしたときのカメラ角度補正速度
	const float zEnemyForcusCorrectionSpeed_ = 0.25f;

	// Z注目時の回転補正値
	float minOffsetRotateY_ = 0.15f; // 最小
	float maxOffsetRotateY_ = 0.55f; // 最大
	// Z注目時のZ軸方向の最大オフセット値
	float maxZOffset_ = -27.0f;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	// カメラの追従遅延量
	const float trackingDelay_ = 0.35f;
	// Z注目時の追従遅延量
	const float zForcusTrackingDelay_ = 0.1f;

	// Z注目有効トリガー
	bool enableZForcus_ = false;

	// ロックオン時の遷移先座標
	Vector3 lockOnTranslate_ = {};

	// ロックオン方向のセットアップフラグ
	bool lockOnDirectionSetUp_ = false;
	// どちらに角度を補正するか
	bool isRightLockOn_ = false;

	// オフセット
	const Vector3 kOffset_ = { 0.0f, 2.0f, -10.0f };
	Vector3 offset_ = kOffset_;
	// 補間時のターゲット用オフセット
	Vector3 targetOffset_ = kOffset_;

	// ロックオン時の回転角
	float kOffsetRotate_ = 0.05f;

	// ロックオン中のビネット補間値
	float vignetteT_		= 0.025f;
	// ロックオン中のビネット強さ
	float vignetteStrength_ = 0.25f;

	// パリィ演出中かのフラグ
	bool isParryStaging_ = false;
	// パリィ時のラジアルブラー演出用タイマー
	KLib::DeltaTimer parryBlurTimer_{};
	// パリィ時のブラー強さ
	float parryBlurStrength_ = 0.55f;
	// パリィ演出の終了時間
	float parryStagingEndTime_ = 0.0f;
	// 終了演出中フラグ
	bool isParryEndStaging_ = false;
};


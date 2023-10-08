#pragma once
#include "../Math/Math.h"

/// <summary>
/// 平行光源クラス
/// </summary>
class DirectionalLight
{
public: // サブクラス

	/// <summary>
	/// 定数バッファデータ構造体
	/// </summary>
	struct ConstBufferData {
		Vector4 color; // ライト色
		Vector3 direction; // ライト向き
		float intensity; // 輝度
		unsigned int active; // 有効非有効
	};

public: // メンバ関数

	/// <summary>
	/// ライト方向のセッター
	/// </summary>
	/// <param name="direction">ライト向き</param>
	inline void SetLightDirection(const Vector3& direction) { this->lightDirection_ = Math::Normalize(direction); };
	/// <summary>
	/// ライト方向のゲッター
	/// </summary>
	/// <returns>ライト向き</returns>
	inline const Vector3 GetLightDirection() { return lightDirection_; };

	/// <summary>
	/// ライト色セッター
	/// </summary>
	/// <param name="color">ライト色</param>
	inline void SetLightColor(const Vector4& color) { this->lightColor_ = color; }
	/// <summary>
	/// ライト色ゲッター
	/// </summary>
	/// <returns>ライト色</returns>
	inline const Vector4 GetColor() { return lightColor_; }

	/// <summary>
	/// 輝度のセッター
	/// </summary>
	/// <param name="intensity">輝度</param>
	inline void SetIntensity(const float& intensity) { this->intensity_ = intensity; }
	/// <summary>
	/// 輝度のゲッター
	/// </summary>
	/// <returns>輝度</returns>
	inline const float GetIntensity() { return intensity_; }

	/// <summary>
	/// ライト有効状態のセッター
	/// </summary>
	/// <param name="active">有効非有効</param>
	inline void SetActive(bool active) { this->isActive_ = active; }
	/// <summary>
	/// ライト有効状態のゲッター
	/// </summary>
	/// <returns></returns>
	inline const int32_t GetActive() { return isActive_; }

private: // メンバ変数

	// ライト方向
	Vector3 lightDirection_ = { 1.0f, 0.0f, 0.0f };
	// ライト色
	Vector4 lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ライト輝度
	float intensity_ = 1.0f;
	// ライトを有効にするか
	int32_t isActive_ = false;

};


#pragma once

// クラスの前方宣言
class Vector3;

/// <summary>
/// クォータニオン
/// </summary>
class Quaternion final {
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Quaternion();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="v">オイラー角</param>
	Quaternion(const Vector3& v);

public: // 演算子オーバーロード

	inline Vector3& vec();
	inline const Vector3& vec() const;

	/// <summary>
	/// 演算子のオーバーロード(+)
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>積</returns>
	Quaternion operator+ (const Quaternion q) const;

	/// <summary>
	/// 演算子のオーバーロード(*)
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>積</returns>
	Quaternion operator* (const Quaternion q) const;

	/// <summary>
	/// 演算子のオーバーロード(*)
	/// </summary>
	/// <param name="f">float</param>
	/// <returns>積</returns>
	Quaternion operator* (const float f) const;


	/// <summary>
	/// 演算子のオーバーロード(/)
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>除算</returns>
	Quaternion operator/ (const Quaternion q) const;

public: // メンバ変数

	// 実部
	float x;
	float y;
	float z;
	// 虚部
	float w;
};

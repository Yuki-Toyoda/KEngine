#pragma once
#include "Vector3.h"

/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion {
	float x;
	float y;
	float z;
	float w;

	inline Vector3& vec() { return *reinterpret_cast<Vector3*>(&x); }
	inline const Vector3& vec() const { return *reinterpret_cast<const Vector3*>(&x); }

	/// <summary>
	/// 演算子のオーバーロード(*)
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>積</returns>
	Quaternion operator* (const Quaternion q) const;

	/// <summary>
	/// 演算子のオーバーロード(/)
	/// </summary>
	/// <param name="q">クォータニオン</param>
	/// <returns>除算</returns>
	Quaternion operator/ (const Quaternion q) const;

};

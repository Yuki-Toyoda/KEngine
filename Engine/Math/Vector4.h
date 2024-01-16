#pragma once
/// <summary>
/// 4次元ベクトル構造体
/// </summary>
struct Vector4 final {
	float x; // x軸
	float y; // y軸
	float z; // z軸
	float w; // w

	// 演算子オーバーロード(==)
	bool operator==(const Vector4 v) const {
		return { x == v.x && y == v.y && z == v.z && w == v.w };
	}
};

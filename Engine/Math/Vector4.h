#pragma once

/// <summary>
/// 4次元ベクトルクラス
/// </summary>
class Vector4 final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector4() noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">x軸初期値</param>
	/// <param name="y">y軸初期値</param>
	/// <param name="z">z軸初期値</param>
	/// <param name="w">w軸初期値</param>
	Vector4(float x, float y, float z, float w) noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="v">他の4次元ベクトル</param>
	Vector4(const Vector4& v) noexcept;

	// 既定のデストラクタを使用する
	~Vector4() = default;

public: // 静的関数

	/// <summary>
	/// ゼロベクトル
	/// </summary>
	static const Vector4 kZero;

public: // メンバ変数

	// x軸
	float x;
	// y軸
	float y;
	// z軸
	float z;
	// w軸
	float w;

};


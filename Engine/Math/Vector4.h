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
	/// <param name="v_">他の4次元ベクトル</param>
	Vector4(const Vector4& v) noexcept;

	// 既定のデストラクタを使用する
	~Vector4() = default;

public: // 演算子のオーバーロード

	/// <summary>
	/// 値を変更せず、自身をそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の値</returns>
	inline Vector4 operator+() const noexcept {
		// 自分自身を返す
		return *this;
	}
	/// <summary>
	/// 自身の逆ベクトルをそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の逆ベクトル</returns>
	inline Vector4 operator-() const noexcept {
		// 逆の値を返す
		return { -x, -y, -z, -w };
	}

	/// <summary>
	/// 演算子オーバーロード(+)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator+(const Vector4& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(-)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator-(const Vector4& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(*)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator*(const Vector4& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(/)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator/(const Vector4& v) const noexcept;

	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4& operator=(const Vector4& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4& operator=(Vector4&& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(+=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator+=(const Vector4& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(-=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator-=(const Vector4& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(*=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator*=(const Vector4& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(/=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator/=(const Vector4& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator*(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator/(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator*=(const float& f) noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector4 operator/=(const float& f) noexcept;

	/// <summary>
	/// 値が等しいかどうかを調べる
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>等しい or 等しくない</returns>
	bool operator==(const Vector4& v) const noexcept;
	/// <summary>
	/// 値の一致、不一致を調べる
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>一致 or 不一致</returns>
	bool operator!=(const Vector4& v) const noexcept;

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


#pragma once

/// <summary>
/// 2次元ベクトルクラス
/// </summary>
class Vector2 final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector2() noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">x軸初期値</param>
	/// <param name="y">y軸初期値</param>
	Vector2(float x, float y) noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="v">他の2次元ベクトル</param>
	Vector2(const Vector2& v) noexcept;

	// 既定のデストラクタを使用する
	~Vector2() = default;

public: // 演算子のオーバーロード

	/// <summary>
	/// 値を変更せず、自身をそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の値</returns>
	inline Vector2 operator+() const noexcept {
		// 自分自身を返す
		return *this;
	}
	/// <summary>
	/// 自身の逆ベクトルをそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の逆ベクトル</returns>
	inline Vector2 operator-() const noexcept {
		// 逆の値を返す
		return { -x, -y };
	}

	/// <summary>
	/// 演算子オーバーロード(+)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator+(const Vector2& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(-)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator-(const Vector2& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(*)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator*(const Vector2& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(/)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator/(const Vector2& v) const noexcept;

	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2& operator=(const Vector2& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2& operator=(Vector2&& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(+=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator+=(const Vector2& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(-=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator-=(const Vector2& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(*=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator*=(const Vector2& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(/=)
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator/=(const Vector2& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator*(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator/(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator*=(const float& f) noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector2 operator/=(const float& f) noexcept;

	/// <summary>
	/// 値が等しいかどうかを調べる
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>等しい or 等しくない</returns>
	bool operator==(const Vector2& v) const noexcept;
	/// <summary>
	/// 値の一致、不一致を調べる
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>一致 or 不一致</returns>
	bool operator!=(const Vector2& v) const noexcept;

public: // メンバ関数

	/// <summary>
	/// 内積を求める
	/// </summary>
	/// <param name="v">計算するベクトル</param>
	/// <returns>内積</returns>
	float Dot(const Vector2& v) const noexcept;

	/// <summary>
	/// ベクトル長さを求める
	/// </summary>
	/// <returns>ベクトル長さ</returns>
	float Length() const noexcept;

	/// <summary>
	/// クロス積を求める
	/// </summary>
	/// <param name="v">計算するベクトル</param>
	/// <returns>クロス積</returns>
	float Cross(const Vector2& v) const noexcept;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector2 Normalize() const noexcept;

public: // 静的関数

	/// <summary>
	/// ゼロベクトル
	/// </summary>
	static const Vector2 kZero;

public: // メンバ変数

	// x軸
	float x;
	// y軸
	float y;

};

